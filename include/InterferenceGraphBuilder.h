//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_INTERFERENCEGRAPHBUILDER_H
#define DA2_INTERFERENCEGRAPHBUILDER_H

#include <vector>
#include "Graph.h"
#include "Web.h"

class InterferenceGraphBuilder {
public:
    Graph<int> build(const std::vector<Web>& webs) const;

private:
    bool interfere(const Web& a, const Web& b) const;
};

#endif //DA2_INTERFERENCEGRAPHBUILDER_H