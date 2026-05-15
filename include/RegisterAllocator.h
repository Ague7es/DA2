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

class RegisterAllocator {
public:
    AllocationResult allocateBasic(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters) const;
    AllocationResult allocateSpilling(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, int maxSpills) const;

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
};

#endif //DA2_REGISTERALLOCATOR_H