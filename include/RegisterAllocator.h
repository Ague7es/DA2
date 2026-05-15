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

private:
    std::set<int> getNeighbors(const Graph<int>& graph, int webId) const;
    int chooseSpillCandidate(const std::set<int>& activeNodes, const Graph<int>& graph) const;
};

#endif //DA2_REGISTERALLOCATOR_H