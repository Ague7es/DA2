//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_WEB_H
#define DA2_WEB_H

#include <string>
#include <vector>
#include "ProgramPoint.h"

/**
 * @struct Web
 *
 * @brief Represents a web used for register allocation.
 *
 * A web is a group of connected live ranges belonging to the same variable.
 *
 * Webs are used as the vertices of the interference graph during
 * register allocation.
 */
struct Web {
    int id;
    std::string variable;
    std::vector<ProgramPoint> points;
};

#endif //DA2_WEB_H