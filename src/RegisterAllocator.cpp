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