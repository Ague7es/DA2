//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_LIVERANGE_H
#define DA2_LIVERANGE_H

#include <string>
#include <vector>
#include "ProgramPoint.h"

struct LiveRange {
    std::string variable;
    std::vector<ProgramPoint> points;
};

#endif //DA2_LIVERANGE_H