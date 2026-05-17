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

/**
 * @class RegisterParser
 *
 * @brief Parses register allocation configuration files.
 *
 * This parser reads:
 * - the number of available registers;
 * - the allocation algorithm;
 * - optional algorithm parameters.
 *
 * Comment lines are ignored.
 */
class RegisterParser {

public:

    /**
     * @brief Parses a register configuration file.
     *
     * Reads and validates:
     * - number of registers;
     * - allocation algorithm;
     * - optional parameters.
     *
     * @param filename Path to the configuration input file.
     *
     * @return Parsed allocation configuration.
     *
     * @throws std::runtime_error If the file cannot be opened or contains invalid data.
     *
     * Time Complexity:
     * O(N)
     *
     * where:
     * - N = number of lines in the file
     */
    AllocationConfig parse(const std::string& filename) const;

private:

    /**
     * @brief Removes leading and trailing whitespace from a string.
     *
     * @param str Input string.
     *
     * @return Trimmed string.
     *
     * Time Complexity:
     * O(N)
     *
     * where:
     * - N = string length
     */
    static std::string trim(const std::string& str);
};

#endif //DA2_REGISTERPARSER_H