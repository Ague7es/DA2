//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_OUTPUTWRITER_H
#define DA2_OUTPUTWRITER_H

#include <string>
#include <vector>
#include "Web.h"
#include "AllocationResult.h"

class OutputWriter {
public:
    static void write(const std::string& filename, const std::vector<Web>& webs, const AllocationResult& result);

private:
    static void writeWeb(std::ofstream& file, const Web& web);
};

#endif //DA2_OUTPUTWRITER_H