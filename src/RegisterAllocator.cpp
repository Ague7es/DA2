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

// Splits web into two halves at splitIndex.
// partA = points[0 .. splitIndex-1],  partB = points[splitIndex .. end]
// partB gets a fresh id (nextId). Both halves keep the parent variable name.
std::pair<Web, Web> RegisterAllocator::splitWeb(
        const Web& web, int splitIndex, int nextId) const {

    Web partA;
    partA.id       = web.id;
    partA.variable = web.variable;
    partA.points   = std::vector<ProgramPoint>(
                         web.points.begin(),
                         web.points.begin() + splitIndex);

    Web partB;
    partB.id       = nextId;
    partB.variable = web.variable;
    partB.points   = std::vector<ProgramPoint>(
                         web.points.begin() + splitIndex,
                         web.points.end());

    return {partA, partB};
}

// For a given web, scores every possible cut point (1 <= cut < points.size())
// by counting how many interference edges with other webs would be eliminated.
// An edge between this web and another is eliminated when neither half still
// shares a program point with that other web.
// Returns {bestCutIndex, edgesRemoved}.
std::pair<int, int> RegisterAllocator::bestSplitPoint(
        const Web& web,
        const std::vector<Web>& allWebs) const {

    // Pre-build a set of this web's point lines for fast lookup
    std::set<int> allLines;
    for (const auto& p : web.points) allLines.insert(p.line);

    int bestCut   = 1;
    int bestScore = -1;

    int n = static_cast<int>(web.points.size());

    for (int cut = 1; cut < n; cut++) {
        // Build line sets for each half
        std::set<int> linesA, linesB;
        for (int i = 0; i < cut; i++) linesA.insert(web.points[i].line);
        for (int i = cut; i < n; i++) linesB.insert(web.points[i].line);

        int edgesRemoved = 0;

        for (const auto& other : allWebs) {
            if (other.id == web.id) continue;

            // Does 'other' currently interfere with this web?
            bool currentlyInterferes = false;
            for (const auto& p : other.points) {
                if (allLines.count(p.line)) { currentlyInterferes = true; break; }
            }
            if (!currentlyInterferes) continue;

            // Would 'other' still interfere with partA?
            bool interferesA = false;
            for (const auto& p : other.points) {
                if (linesA.count(p.line)) { interferesA = true; break; }
            }

            // Would 'other' still interfere with partB?
            bool interferesB = false;
            for (const auto& p : other.points) {
                if (linesB.count(p.line)) { interferesB = true; break; }
            }

            // The original edge becomes two edges (A–other, B–other) if both
            // halves still interfere, or one edge if only one half does,
            // or zero if neither half does.
            // Net change = edges_after - 1 (the original).
            // We want to maximise edges removed = 1 - edges_after.
            int edgesAfter = (interferesA ? 1 : 0) + (interferesB ? 1 : 0);
            edgesRemoved += (1 - edgesAfter); // can be negative if split adds an edge
        }

        if (edgesRemoved > bestScore) {
            bestScore = edgesRemoved;
            bestCut   = cut;
        }
    }

    return {bestCut, bestScore};
}

// Picks the web to split next: the candidate whose best cut removes the most
// interference edges. Among ties, prefer the web with the highest current degree
// (most neighbours = highest benefit from any reduction).
// Returns {index into currentWebs, bestCutIndex for that web}.
std::pair<int, int> RegisterAllocator::chooseSplitCandidate(
        const std::vector<Web>& currentWebs) const {

    int bestWebIdx  = -1;
    int bestCutIdx  = -1;
    int bestScore   = std::numeric_limits<int>::min();
    int bestDegree  = -1; // tiebreaker: higher degree preferred

    for (int i = 0; i < static_cast<int>(currentWebs.size()); i++) {
        const Web& web = currentWebs[i];
        if (static_cast<int>(web.points.size()) < 2) continue; // unsplittable

        auto [cut, score] = bestSplitPoint(web, currentWebs);

        // Compute current degree of this web (number of interfering webs)
        std::set<int> myLines;
        for (const auto& p : web.points) myLines.insert(p.line);

        int degree = 0;
        for (const auto& other : currentWebs) {
            if (other.id == web.id) continue;
            for (const auto& p : other.points) {
                if (myLines.count(p.line)) { degree++; break; }
            }
        }

        bool better = (score > bestScore) ||
                      (score == bestScore && degree > bestDegree);

        if (better) {
            bestScore  = score;
            bestDegree = degree;
            bestWebIdx = i;
            bestCutIdx = cut;
        }
    }

    return {bestWebIdx, bestCutIdx};
}

// ---------------------------------------------------------------------------
// allocateWithSplitting
// ---------------------------------------------------------------------------

// Tries to allocate registers, splitting up to maxSplits webs if needed.
// Splitting heuristic: at each step, pick the web and cut point that removes
// the most interference edges. The graph is rebuilt after every split.
// On failure after maxSplits, all current webs (including split ones) → M.
AllocationResult RegisterAllocator::allocateWithSplitting(
        const std::vector<Web>& webs,
        int numberOfRegisters,
        int maxSplits) const {

    InterferenceGraphBuilder graphBuilder;

    // Work on a mutable copy of the web list
    std::vector<Web> currentWebs = webs;

    // Track the next available web id (beyond any existing id)
    int nextId = 0;
    for (const auto& w : currentWebs) nextId = std::max(nextId, w.id + 1);

    // Try with zero splits first
    {
        Graph<int> graph = graphBuilder.build(currentWebs);
        AllocationResult result = tryColorWithSpills(graph, currentWebs,
                                                     numberOfRegisters, {});
        if (result.success) { result.finalWebs = currentWebs; return result; }
    }

    // Incrementally split one web at a time, up to maxSplits
    for (int splitsDone = 0; splitsDone < maxSplits; splitsDone++) {

        auto [webIdx, cutIdx] = chooseSplitCandidate(currentWebs);

        if (webIdx == -1) break; // no splittable web left

        // Perform the split
        auto [partA, partB] = splitWeb(currentWebs[webIdx], cutIdx, nextId++);

        // Replace the original web with the two halves in-place
        currentWebs.erase(currentWebs.begin() + webIdx);
        currentWebs.push_back(partA);
        currentWebs.push_back(partB);

        // Rebuild graph and try coloring
        Graph<int> graph = graphBuilder.build(currentWebs);
        AllocationResult result = tryColorWithSpills(graph, currentWebs,
                                                     numberOfRegisters, {});
        if (result.success) { result.finalWebs = currentWebs; return result; }
    }

    // Failed even after maxSplits splits — all current webs (split included) → M
    AllocationResult failed;
    failed.success       = false;
    failed.registersUsed = 0;
    failed.finalWebs     = currentWebs;
    for (const auto& w : currentWebs) failed.spilledWebs.insert(w.id);
    return failed;
}
