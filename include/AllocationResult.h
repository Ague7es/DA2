//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_ALLOCATIONRESULT_H
#define DA2_ALLOCATIONRESULT_H

#include <map>
#include <set>

struct AllocationResult {
    bool success = false;
    int registersUsed = 0;

    std::map<int, int> webToRegister;
    std::set<int> spilledWebs;

    // Populated by allocateWithSplitting: the web list after all splits.
    // Empty for basic/spilling (original web list is unchanged there).
    std::vector<Web> finalWebs;
};

#endif //DA2_ALLOCATIONRESULT_H