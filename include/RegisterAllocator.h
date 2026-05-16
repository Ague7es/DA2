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
    std::vector<int> rankSpillCandidates(const Graph<int>& graph, const std::vector<Web>& webs) const;
    AllocationResult tryColorWithSpills(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, const std::set<int>& spilledIds) const;

    std::pair<Web, Web> splitWeb(const Web& web, int splitIndex, int nextId) const;
    std::pair<int, int> bestSplitPoint(const Web& web, const std::vector<Web>& allWebs) const;
    std::pair<int, int> chooseSplitCandidate(const std::vector<Web>& currentWebs) const;
};

#endif //DA2_REGISTERALLOCATOR_H