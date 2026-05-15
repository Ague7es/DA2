//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/RegisterAllocator.h"
#include <algorithm>

AllocationResult RegisterAllocator::allocateBasic(const Graph<int>& graph,const std::vector<Web>& webs, int numberOfRegisters) const {
    AllocationResult result;

    std::set<int> activeNodes;
    std::stack<int> stack;

    for (const auto& web : webs) {
        activeNodes.insert(web.id);
    }

    std::set<int> potentialSpills;

    while (!activeNodes.empty()) {
        bool removedNode = false;

        for (auto it = activeNodes.begin(); it != activeNodes.end(); ++it) {
            int node = *it;

            int activeDegree = 0;
            auto neighbors = getNeighbors(graph, node);

            for (int neighbor : neighbors) {
                if (activeNodes.count(neighbor)) {
                    activeDegree++;
                }
            }

            if (activeDegree < numberOfRegisters) {
                stack.push(node);
                activeNodes.erase(it);
                removedNode = true;
                break;
            }
        }

        if (!removedNode) {
            int spillCandidate = chooseSpillCandidate(activeNodes, graph);
            potentialSpills.insert(spillCandidate);
            activeNodes.erase(spillCandidate);
        }
    }

    std::map<int, int> colors;

    while (!stack.empty()) {
        int node = stack.top();
        stack.pop();

        std::set<int> usedColors;

        auto neighbors = getNeighbors(graph, node);

        for (int neighbor : neighbors) {
            if (colors.count(neighbor)) {
                usedColors.insert(colors[neighbor]);
            }
        }

        int chosenColor = -1;

        for (int color = 0; color < numberOfRegisters; color++) {
            if (!usedColors.count(color)) {
                chosenColor = color;
                break;
            }
        }

        if (chosenColor == -1) {
            result.success = false;
            result.registersUsed = 0;

            for (const auto& web : webs) {
                result.spilledWebs.insert(web.id);
            }

            return result;
        }

        colors[node] = chosenColor;
    }

    if (!potentialSpills.empty()) {
        result.success = false;
        result.registersUsed = 0;

        for (const auto& web : webs) {
            result.spilledWebs.insert(web.id);
        }

        return result;
    }

    result.success = true;
    result.webToRegister = colors;

    int maxRegister = -1;
    for (const auto& pair : colors) {
        maxRegister = std::max(maxRegister, pair.second);
    }

    result.registersUsed = maxRegister + 1;

    return result;
}

std::set<int> RegisterAllocator::getNeighbors(const Graph<int>& graph, int webId) const {
    std::set<int> neighbors;

    auto vertex = graph.findVertex(webId);

    if (vertex == nullptr) {
        return neighbors;
    }

    for (auto edge : vertex->getAdj()) {
        neighbors.insert(edge->getDest()->getInfo());
    }

    return neighbors;
}

int RegisterAllocator::chooseSpillCandidate(const std::set<int>& activeNodes,
                                            const Graph<int>& graph) const {
    int bestNode = -1;
    int bestDegree = -1;

    for (int node : activeNodes) {
        int degree = 0;
        auto neighbors = getNeighbors(graph, node);

        for (int neighbor : neighbors) {
            if (activeNodes.count(neighbor)) {
                degree++;
            }
        }

        if (degree > bestDegree) {
            bestDegree = degree;
            bestNode = node;
        }
    }

    return bestNode;
}

// Ranks all webs by spill desirability:
//   Primary key:   highest degree in the interference graph (most conflicts removed)
//   Tiebreaker:    smallest web (fewest program points = cheapest to spill)
std::vector<int> RegisterAllocator::rankSpillCandidates(
        const Graph<int>& graph,
        const std::vector<Web>& webs) const {

    // Build a lookup from web id -> number of program points
    std::map<int, int> webSize;
    for (const auto& web : webs) {
        webSize[web.id] = static_cast<int>(web.points.size());
    }

    // Collect (webId, degree) pairs
    std::vector<std::pair<int,int>> ranked; // (webId, degree)
    for (const auto& web : webs) {
        int degree = static_cast<int>(getNeighbors(graph, web.id).size());
        ranked.push_back({web.id, degree});
    }

    // Sort: higher degree first; on tie, smaller web first
    std::sort(ranked.begin(), ranked.end(),
        [&webSize](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            if (a.second != b.second) return a.second > b.second; // higher degree first
            return webSize.at(a.first) < webSize.at(b.first);     // smaller web first
        });

    std::vector<int> result;
    result.reserve(ranked.size());
    for (const auto& p : ranked) {
        result.push_back(p.first);
    }
    return result;
}

// Attempts graph coloring ignoring the webs in spilledIds.
// Returns a successful AllocationResult if coloring succeeds, failure otherwise.
AllocationResult RegisterAllocator::tryColorWithSpills(
        const Graph<int>& graph,
        const std::vector<Web>& webs,
        int numberOfRegisters,
        const std::set<int>& spilledIds) const {

    AllocationResult result;

    std::set<int> activeNodes;
    for (const auto& web : webs) {
        if (!spilledIds.count(web.id)) {
            activeNodes.insert(web.id);
        }
    }

    // Simplification phase: push nodes with active-degree < N onto the stack
    std::stack<int> stack;

    while (!activeNodes.empty()) {
        bool removedNode = false;

        for (auto it = activeNodes.begin(); it != activeNodes.end(); ++it) {
            int node = *it;
            int activeDegree = 0;

            for (int neighbor : getNeighbors(graph, node)) {
                if (activeNodes.count(neighbor)) {
                    activeDegree++;
                }
            }

            if (activeDegree < numberOfRegisters) {
                stack.push(node);
                activeNodes.erase(it);
                removedNode = true;
                break;
            }
        }

        // If no node could be simplified, the remaining graph is too dense
        // even after spilling — coloring will fail.
        if (!removedNode) {
            result.success = false;
            return result;
        }
    }

    // Coloring phase: pop and assign the lowest available color
    std::map<int, int> colors;

    while (!stack.empty()) {
        int node = stack.top();
        stack.pop();

        std::set<int> usedColors;
        for (int neighbor : getNeighbors(graph, node)) {
            if (colors.count(neighbor)) {
                usedColors.insert(colors[neighbor]);
            }
        }

        int chosenColor = -1;
        for (int color = 0; color < numberOfRegisters; color++) {
            if (!usedColors.count(color)) {
                chosenColor = color;
                break;
            }
        }

        if (chosenColor == -1) {
            result.success = false;
            return result;
        }

        colors[node] = chosenColor;
    }

    result.success = true;
    result.webToRegister = colors;
    result.spilledWebs = spilledIds;

    int maxRegister = -1;
    for (const auto& pair : colors) {
        maxRegister = std::max(maxRegister, pair.second);
    }
    result.registersUsed = maxRegister + 1;

    return result;
}

// Tries to allocate registers, spilling up to maxSpills webs if needed.
// Spill candidates are chosen greedily: highest-degree first, ties broken
// by smallest web (cheapest to commit to memory).
AllocationResult RegisterAllocator::allocateSpilling(
        const Graph<int>& graph,
        const std::vector<Web>& webs,
        int numberOfRegisters,
        int maxSpills) const {

    // First try with no spills at all
    std::set<int> spilledIds;
    AllocationResult result = tryColorWithSpills(graph, webs, numberOfRegisters, spilledIds);
    if (result.success) return result;

    // Rank candidates once — order is stable across all iterations
    std::vector<int> candidates = rankSpillCandidates(graph, webs);

    // Incrementally spill one more candidate at a time, up to maxSpills
    for (int spillCount = 1; spillCount <= maxSpills; spillCount++) {
        if (spillCount > static_cast<int>(candidates.size())) break;

        spilledIds.insert(candidates[spillCount - 1]);

        result = tryColorWithSpills(graph, webs, numberOfRegisters, spilledIds);
        if (result.success) return result;
    }

    // Could not color even after maxSpills spills — report full failure
    result.success = false;
    result.registersUsed = 0;
    result.webToRegister.clear();
    result.spilledWebs.clear();
    for (const auto& web : webs) {
        result.spilledWebs.insert(web.id);
    }
    return result;
}