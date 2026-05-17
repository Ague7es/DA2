//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_ALLOCATIONRESULT_H
#define DA2_ALLOCATIONRESULT_H

#include <map>
#include <set>

/**
 * @struct AllocationResult
 *
 * @brief Stores the result of a register allocation execution.
 *
 * This structure contains:
 * - whether the allocation was successful;
 * - the number of registers effectively used;
 * - the mapping between webs and assigned registers;
 * - the set of spilled webs;
 * - the final set of webs after optional splitting operations.
 */
struct AllocationResult {
    bool success = false;
    int registersUsed = 0;

    std::map<int, int> webToRegister;
    std::set<int> spilledWebs;

    std::vector<Web> finalWebs;
};

#endif //DA2_ALLOCATIONRESULT_H