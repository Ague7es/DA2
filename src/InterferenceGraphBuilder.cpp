//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/InterferenceGraphBuilder.h"

Graph<int> InterferenceGraphBuilder::build(const std::vector<Web>& webs) const {
    Graph<int> graph;

    for (const auto& web : webs) {
        graph.addVertex(web.id);
    }

    for (size_t i = 0; i < webs.size(); i++) {
        for (size_t j = i + 1; j < webs.size(); j++) {
            if (interfere(webs[i], webs[j])) {
                graph.addBidirectionalEdge(webs[i].id, webs[j].id, 1.0);
            }
        }
    }

    return graph;
}

bool InterferenceGraphBuilder::interfere(const Web& a, const Web& b) const {
    for (const auto& pointA : a.points) {
        for (const auto& pointB : b.points) {
            if (pointA.line == pointB.line) {

                bool aEndsAndBStarts = pointA.isEnd && pointB.isStart;
                bool bEndsAndAStarts = pointB.isEnd && pointA.isStart;

                if (aEndsAndBStarts || bEndsAndAStarts) {
                    continue;
                }

                return true;
            }
        }
    }

    return false;
}