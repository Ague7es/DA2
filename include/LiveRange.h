//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_LIVERANGE_H
#define DA2_LIVERANGE_H

#include <string>
#include <vector>
#include "ProgramPoint.h"

/**
 * @struct LiveRange
 *
 * @brief Represents the live range of a variable.
 *
 * A live range describes the interval in which a variable
 * is alive during program execution.
 *
 * It is composed of:
 * - the variable name;
 * - a sequence of program points where the variable is live.
 */
struct LiveRange {
    std::string variable;
    std::vector<ProgramPoint> points;
};

#endif //DA2_LIVERANGE_H