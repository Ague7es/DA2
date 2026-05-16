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

int RegisterAllocator::chooseSpillCandidate(const std::set<int>& activeNodes, const Graph<int>& graph) const {
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

std::vector<int> RegisterAllocator::rankSpillCandidates(const Graph<int>& graph, const std::vector<Web>& webs) const {
    std::map<int, int> webSize;
    for (const auto& web : webs) {
        webSize[web.id] = static_cast<int>(web.points.size());
    }

    std::vector<std::pair<int,int>> ranked;
    for (const auto& web : webs) {
        int degree = static_cast<int>(getNeighbors(graph, web.id).size());
        ranked.push_back({web.id, degree});
    }

    std::sort(ranked.begin(), ranked.end(), [&webSize](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            if (a.second != b.second) return a.second > b.second;
            return webSize.at(a.first) < webSize.at(b.first);
        });

    std::vector<int> result;
    result.reserve(ranked.size());
    for (const auto& p : ranked) {
        result.push_back(p.first);
    }
    return result;
}

AllocationResult RegisterAllocator::tryColorWithSpills(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, const std::set<int>& spilledIds) const {
    AllocationResult result;

    std::set<int> activeNodes;
    for (const auto& web : webs) {
        if (!spilledIds.count(web.id)) {
            activeNodes.insert(web.id);
        }
    }

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

        if (!removedNode) {
            result.success = false;
            return result;
        }
    }

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

AllocationResult RegisterAllocator::allocateSpilling(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, int maxSpills) const {

    std::set<int> spilledIds;
    AllocationResult result = tryColorWithSpills(graph, webs, numberOfRegisters, spilledIds);
    if (result.success) return result;

    std::vector<int> candidates = rankSpillCandidates(graph, webs);

    for (int spillCount = 1; spillCount <= maxSpills; spillCount++) {
        if (spillCount > static_cast<int>(candidates.size())) break;

        spilledIds.insert(candidates[spillCount - 1]);
        result = tryColorWithSpills(graph, webs, numberOfRegisters, spilledIds);

        if (result.success) return result;
    }

    result.success = false;
    result.registersUsed = 0;
    result.webToRegister.clear();
    result.spilledWebs.clear();
    for (const auto& web : webs) {
        result.spilledWebs.insert(web.id);
    }
    return result;
}

std::pair<Web, Web> RegisterAllocator::splitWeb(const Web& web, int splitIndex, int nextId) const {

    Web partA;
    partA.id = web.id;
    partA.variable = web.variable;
    partA.points = std::vector<ProgramPoint>(web.points.begin(), web.points.begin() + splitIndex);

    Web partB;
    partB.id = nextId;
    partB.variable = web.variable;
    partB.points = std::vector<ProgramPoint>(web.points.begin() + splitIndex,web.points.end());

    return {partA, partB};
}

std::pair<int, int> RegisterAllocator::bestSplitPoint(const Web& web, const std::vector<Web>& allWebs) const {
    std::set<int> allLines;
    for (const auto& p : web.points) allLines.insert(p.line);

    int bestCut   = 1;
    int bestScore = -1;

    int n = static_cast<int>(web.points.size());

    for (int cut = 1; cut < n; cut++) {
        std::set<int> linesA, linesB;
        for (int i = 0; i < cut; i++) linesA.insert(web.points[i].line);
        for (int i = cut; i < n; i++) linesB.insert(web.points[i].line);

        int edgesRemoved = 0;

        for (const auto& other : allWebs) {
            if (other.id == web.id) continue;

            bool currentlyInterferes = false;
            for (const auto& p : other.points) {
                if (allLines.count(p.line)) {
                    currentlyInterferes = true;
                    break;
                }
            }
            if (!currentlyInterferes) continue;

            bool interferesA = false;
            for (const auto& p : other.points) {
                if (linesA.count(p.line)) { interferesA = true; break; }
            }

            bool interferesB = false;
            for (const auto& p : other.points) {
                if (linesB.count(p.line)) {
                    interferesB = true;
                    break;
                }
            }

            int edgesAfter = (interferesA ? 1 : 0) + (interferesB ? 1 : 0);
            edgesRemoved += (1 - edgesAfter);
        }

        if (edgesRemoved > bestScore) {
            bestScore = edgesRemoved;
            bestCut   = cut;
        }
    }

    return {bestCut, bestScore};
}

std::pair<int, int> RegisterAllocator::chooseSplitCandidate( const std::vector<Web>& currentWebs) const {
    int bestWebIdx  = -1;
    int bestCutIdx  = -1;
    int bestScore   = std::numeric_limits<int>::min();
    int bestDegree  = -1;

    for (int i = 0; i < static_cast<int>(currentWebs.size()); i++) {
        const Web& web = currentWebs[i];
        if (static_cast<int>(web.points.size()) < 2) continue;

        auto [cut, score] = bestSplitPoint(web, currentWebs);

        std::set<int> myLines;
        for (const auto& p : web.points) myLines.insert(p.line);

        int degree = 0;
        for (const auto& other : currentWebs) {
            if (other.id == web.id) continue;
            for (const auto& p : other.points) {
                if (myLines.count(p.line)) degree++; break;
            }
        }

        bool better = (score > bestScore) || (score == bestScore && degree > bestDegree);

        if (better) {
            bestScore  = score;
            bestDegree = degree;
            bestWebIdx = i;
            bestCutIdx = cut;
        }
    }

    return {bestWebIdx, bestCutIdx};
}

AllocationResult RegisterAllocator::allocateWithSplitting(const std::vector<Web>& webs, int numberOfRegisters, int maxSplits) const {

    InterferenceGraphBuilder graphBuilder;

    std::vector<Web> currentWebs = webs;

    int nextId = 0;
    for (const auto& w : currentWebs) nextId = std::max(nextId, w.id + 1);

    {
        Graph<int> graph = graphBuilder.build(currentWebs);
        AllocationResult result = tryColorWithSpills(graph, currentWebs, numberOfRegisters, {});
        if (result.success) {
            result.finalWebs = currentWebs;
            return result;
        }
    }

    for (int splitsDone = 0; splitsDone < maxSplits; splitsDone++) {
        auto [webIdx, cutIdx] = chooseSplitCandidate(currentWebs);
        if (webIdx == -1) break;
        auto [partA, partB] = splitWeb(currentWebs[webIdx], cutIdx, nextId++);

        currentWebs.erase(currentWebs.begin() + webIdx);
        currentWebs.push_back(partA);
        currentWebs.push_back(partB);

        Graph<int> graph = graphBuilder.build(currentWebs);
        AllocationResult result = tryColorWithSpills(graph, currentWebs, numberOfRegisters, {});

        if (result.success) {
            result.finalWebs = currentWebs;
            return result;
        }
    }

    AllocationResult failed;
    failed.success       = false;
    failed.registersUsed = 0;
    failed.finalWebs     = currentWebs;
    for (const auto& w : currentWebs) failed.spilledWebs.insert(w.id);
    return failed;
}
