//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_REGISTERPARSER_H
#define DA2_REGISTERPARSER_H

#include <string>
#include <vector>
#include "LiveRange.h"

#include <string>

#include "AllocationConfig.h"

class RegisterParser {

public:
    AllocationConfig parse(const std::string& filename) const;

private:
    static std::string trim(const std::string& str);
};

#endif //DA2_REGISTERPARSER_H