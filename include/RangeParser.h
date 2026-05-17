//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_RANGEPARSER_H
#define DA2_RANGEPARSER_H

#include <string>
#include <vector>
#include "LiveRange.h"



/**
 * @class RangeParser
 *
 * @brief Parses live range input files.
 *
 * This parser reads the ranges specification file and converts it into
 * internal LiveRange structures used by the application.
 *
 * Comment lines are ignored.
 */
class RangeParser {
public:

    /**
     * @brief Parses a live ranges file.
     *
     * Reads all live ranges and converts them into structured objects.
     *
     * @param filename Path to the ranges input file.
     *
     * @return Vector containing all parsed live ranges.
     *
     * @throws std::runtime_error If the file cannot be opened or contains invalid data.
     *
     * Time Complexity:
     * O(R * P)
     *
     * where:
     * - R = number of live ranges
     * - P = average number of program points per range
     */
    std::vector<LiveRange> parse(const std::string& filename) const;

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

    /**
     * @brief Parses a single program point token.
     *
     * Examples:
     * - "7+"
     * - "10-"
     * - "5"
     *
     * @param token Token representing a program point.
     *
     * @return Parsed ProgramPoint object.
     *
     * @throws std::runtime_error If the token format is invalid.
     *
     * Time Complexity:
     * O(1)
     */
    static ProgramPoint parsePoint(const std::string& token);
};

#endif //DA2_RANGEPARSER_H