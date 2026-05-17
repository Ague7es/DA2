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

/**
 * @class RegisterAllocator
 *
 * @brief Implements register allocation algorithms based on graph coloring.
 *
 * This class supports:
 * - basic graph coloring allocation;
 * - spilling heuristics;
 * - web splitting heuristics.
 *
 * The allocation process operates over an interference graph,
 * where each web is represented as a graph vertex.
 */
class RegisterAllocator {
public:

    /**
     * @brief Performs basic register allocation using graph coloring.
     *
     * Uses:
     * - simplification stack;
     * - greedy coloring.
     *
     * @param graph Interference graph.
     * @param webs Program webs.
     * @param numberOfRegisters Number of available registers.
     *
     * @return Allocation result.
     *
     * Time Complexity:
     * O(V² + VE)
     *
     * where:
     * - V = number of webs
     * - E = number of interference edges
     */
    AllocationResult allocateBasic(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters) const;

    /**
     * @brief Performs register allocation with spilling.
     *
     * If coloring fails, candidate webs are selected and assigned
     * to memory according to a spill heuristic.
     *
     * @param graph Interference graph.
     * @param webs Program webs.
     * @param numberOfRegisters Number of available registers.
     * @param maxSpills Maximum number of allowed spills.
     *
     * @return Allocation result.
     *
     * Time Complexity:
     * Approximately O(S * (V² + VE))
     *
     * where:
     * - S = number of spill attempts
     * - V = number of webs
     * - E = number of edges
     */
    AllocationResult allocateSpilling(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, int maxSpills) const;

    /**
     * @brief Performs register allocation using web splitting.
     *
     * When allocation fails, selected webs are split into smaller webs
     * in order to reduce interference.
     *
     * @param webs Program webs.
     * @param numberOfRegisters Number of available registers.
     * @param maxSplits Maximum number of allowed splits.
     *
     * @return Allocation result.
     *
     * Time Complexity:
     * Approximately O(S * W² * P²)
     *
     * where:
     * - S = number of splits
     * - W = number of webs
     * - P = average number of program points
     */
    AllocationResult allocateWithSplitting(const std::vector<Web>& webs, int numberOfRegisters, int maxSplits) const;

private:

    /**
     * @brief Retrieves the neighbors of a web in the interference graph.
     *
     * @param graph Interference graph.
     * @param webId Web identifier.
     *
     * @return Set of neighboring web identifiers.
     *
     * Time Complexity:
     * O(E)
     */
    std::set<int> getNeighbors(const Graph<int>& graph, int webId) const;

    /**
     * @brief Chooses a spill candidate using a degree heuristic.
     *
     * The web with the highest active degree is selected.
     *
     * @param activeNodes Current active graph nodes.
     * @param graph Interference graph.
     *
     * @return Selected spill candidate identifier.
     *
     * Time Complexity:
     * O(V²)
     */
    int chooseSpillCandidate(const std::set<int>& activeNodes, const Graph<int>& graph) const;

    /**
     * @brief Ranks spill candidates according to heuristics.
     *
     * Webs are ranked primarily by degree and secondarily by web size.
     *
     * @param graph Interference graph.
     * @param webs Program webs.
     *
     * @return Ordered vector of spill candidate identifiers.
     *
     * Time Complexity:
     * O(V² log V)
     */
    std::vector<int> rankSpillCandidates(const Graph<int>& graph, const std::vector<Web>& webs) const;

    /**
     * @brief Attempts graph coloring while ignoring spilled webs.
     *
     * @param graph Interference graph.
     * @param webs Program webs.
     * @param numberOfRegisters Number of available registers.
     * @param spilledIds Set of spilled web identifiers.
     *
     * @return Allocation result.
     *
     * Time Complexity:
     * O(V² + VE)
     */
    AllocationResult tryColorWithSpills(const Graph<int>& graph, const std::vector<Web>& webs, int numberOfRegisters, const std::set<int>& spilledIds) const;

    /**
     * @brief Splits a web into two smaller webs.
     *
     * @param web Original web.
     * @param splitIndex Position where the split occurs.
     * @param nextId Identifier assigned to the new web.
     *
     * @return Pair containing the resulting split webs.
     *
     * Time Complexity:
     * O(P)
     */
    std::pair<Web, Web> splitWeb(const Web& web, int splitIndex, int nextId) const;

    /**
     * @brief Determines the best split point for a web.
     *
     * Uses a score heuristic based on interference reduction.
     *
     * @param web Target web.
     * @param allWebs Current set of webs.
     *
     * @return Pair containing:
     * - best split index;
     * - achieved score.
     *
     * Time Complexity:
     * O(W * P²)
     */
    std::pair<int, int> bestSplitPoint(const Web& web, const std::vector<Web>& allWebs) const;

    /**
     * @brief Selects the best web and split point to apply splitting.
     *
     * Uses:
     * - split score;
     * - web degree.
     *
     * @param currentWebs Current program webs.
     *
     * @return Pair containing:
     * - selected web index;
     * - split point index.
     *
     * Time Complexity:
     * O(W² * P²)
     */
    std::pair<int, int> chooseSplitCandidate(const std::vector<Web>& currentWebs) const;
};

#endif //DA2_REGISTERALLOCATOR_H