//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_ALLOCATIONCONFIG_H
#define DA2_ALLOCATIONCONFIG_H

#include <string>

struct AllocationConfig {
    int numberOfRegisters = 0;
    std::string algorithm = "basic";
    int parameter = 0;
};

#endif //DA2_ALLOCATIONCONFIG_H