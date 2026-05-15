//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_REGISTERALLOCATOR_H
#define DA2_REGISTERALLOCATOR_H

#include <vector>
#include <map>
#include <set>
#include <stack>
#include "Graph.h"
#include "Web.h"
#include "AllocationConfig.h"
#include "AllocationResult.h"
#include "InterferenceGraphBuilder.h"

class RegisterAllocator {
public:
    AllocationResult allocateBasic(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters) const;
    AllocationResult allocateSpilling(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, int maxSpills) const;
    AllocationResult allocateWithSplitting(const std::vector<Web>& webs, int numberOfRegisters, int maxSplits) const;

private:
    std::set<int> getNeighbors(const Graph<int>& graph, int webId) const;
    int chooseSpillCandidate(const std::set<int>& activeNodes, const Graph<int>& graph) const;

    // Returns web ids sorted by spill priority: highest degree first,
    // ties broken by smallest web (fewer program points = cheaper to spill).
    std::vector<int> rankSpillCandidates(const Graph<int>& graph, const std::vector<Web>& webs) const;

    // Attempts graph coloring with a fixed set of pre-spilled webs excluded.
    // Returns a filled AllocationResult on success, or an empty one on failure.
    AllocationResult tryColorWithSpills(const Graph<int>& graph, const std::vector<Web>& webs,
                                        int numberOfRegisters, const std::set<int>& spilledIds) const;

    // Splits web into two halves at splitIndex (points[0..splitIndex-1] and points[splitIndex..end]).
    // partB gets nextId as its new web id. Both halves keep the parent variable name.
    // Returns {partA, partB}.
    std::pair<Web, Web> splitWeb(const Web& web, int splitIndex, int nextId) const;

    // For a given web, finds the cut index (1 <= cut < points.size()) that removes
    // the most interference edges when the web is split there.
    // Also returns the number of edges that would be removed (for ranking across webs).
    std::pair<int, int> bestSplitPoint(const Web& web, const std::vector<Web>& allWebs) const;

    // Picks the web to split next: the candidate whose best cut removes the most edges.
    // Returns {webIndex into currentWebs, bestCutIndex}.
    std::pair<int, int> chooseSplitCandidate(const std::vector<Web>& currentWebs) const;
};

#endif //DA2_REGISTERALLOCATOR_H