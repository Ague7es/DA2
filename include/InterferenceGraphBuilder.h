//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_INTERFERENCEGRAPHBUILDER_H
#define DA2_INTERFERENCEGRAPHBUILDER_H

#include <vector>
#include "Graph.h"
#include "Web.h"

/**
 * @class InterferenceGraphBuilder
 *
 * @brief Builds the interference graph used for register allocation.
 *
 * Each web is represented as a vertex in the graph.
 * An edge is created between two webs when they interfere,
 * meaning they are simultaneously live and therefore cannot
 * share the same register.
 */
class InterferenceGraphBuilder {
public:

    /**
     * @brief Builds the interference graph from a set of webs.
     *
     * Each web becomes a vertex.
     * Interfering webs are connected through bidirectional edges.
     *
     * @param webs Vector containing all program webs.
     *
     * @return Interference graph.
     *
     * Time Complexity:
     * O(W² * P²)
     *
     * where:
     * - W = number of webs
     * - P = average number of program points per web
     */
    Graph<int> build(const std::vector<Web>& webs) const;

private:

    /**
     * @brief Determines whether two webs interfere.
     *
     * Two webs interfere when they are simultaneously live
     * at the same program point.
     *
     * Special care is taken for adjacent ranges such as:
     * - x-
     * - x+
     *
     * which do not necessarily interfere.
     *
     * @param a First web.
     * @param b Second web.
     *
     * @return True if the webs interfere, false otherwise.
     *
     * Time Complexity:
     * O(P²)
     *
     * where:
     * - P = number of program points
     */
    bool interfere(const Web& a, const Web& b) const;
};

#endif //DA2_INTERFERENCEGRAPHBUILDER_H