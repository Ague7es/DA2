//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_RANGEPARSER_H
#define DA2_RANGEPARSER_H

#include <string>
#include <vector>
#include "LiveRange.h"

class RangeParser {
public:
    std::vector<LiveRange> parse(const std::string& filename) const;

private:
    static std::string trim(const std::string& str);
    static ProgramPoint parsePoint(const std::string& token);
};

#endif //DA2_RANGEPARSER_H