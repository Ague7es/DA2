//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_ALLOCATIONCONFIG_H
#define DA2_ALLOCATIONCONFIG_H

#include <string>

/**
 * @struct AllocationConfig
 *
 * @brief Stores the register allocation configuration parsed from the input file.
 *
 * This structure contains:
 * - the number of available registers;
 * - the allocation algorithm to use;
 * - an optional numeric parameter used by spilling or splitting algorithms.
 */
struct AllocationConfig {
    int numberOfRegisters = 0;
    std::string algorithm = "basic";
    int parameter = 0;
};

#endif //DA2_ALLOCATIONCONFIG_H