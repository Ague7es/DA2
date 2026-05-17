//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_OUTPUTWRITER_H
#define DA2_OUTPUTWRITER_H

#include <string>
#include <vector>
#include "Web.h"
#include "AllocationResult.h"

/**

 * @class OutputWriter
 *
 * @brief Responsible for writing allocation results to an output file.
 *
 * The generated file follows the specification defined in the project statement,
 * including:
 * - generated webs;
 * - register assignments;
 * - spilled webs.
 */
class OutputWriter {
public:

    /**
     * @brief Writes the allocation result to a file.
     *
     * The output includes:
     * - all generated webs;
     * - assigned registers;
     * - spilled webs when allocation fails.
     *
     * Webs are sorted for presentation purposes before being written.
     *
     * @param filename Output file path.
     * @param webs Vector containing all program webs.
     * @param result Allocation execution result.
     *
     * @throws std::runtime_error If the file cannot be opened.
     *
     * Time Complexity:
     * O(W log W + W * P)
     *
     * where:
     * - W = number of webs
     * - P = average number of program points per web
     */
    static void write(const std::string& filename, const std::vector<Web>& webs, const AllocationResult& result);

private:

    /**
     * @brief Writes a single web to the output file.
     *
     * @param file Output file stream.
     * @param web Web to be written.
     * @param printedId Identifier used for output formatting.
     *
     * Time Complexity:
     * O(P)
     *
     * where:
     * - P = number of program points in the web
     */
    static void writeWeb(std::ofstream& file, const Web& web, int printedId);
};

#endif //DA2_OUTPUTWRITER_H