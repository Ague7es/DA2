//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_WEBBUILDER_H
#define DA2_WEBBUILDER_H

#include <vector>
#include "LiveRange.h"
#include "Web.h"

/**
 * @class WebBuilder
 *
 * @brief Builds webs from live ranges.
 *
 * Live ranges belonging to the same variable are merged into the same web
 * whenever they overlap or share common program points.
 */
class WebBuilder {
public:

    /**
     * @brief Builds webs from a set of live ranges.
     *
     * Overlapping live ranges associated with the same variable
     * are merged into a single web.
     *
     * @param liveRanges Vector containing all parsed live ranges.
     *
     * @return Vector of generated webs.
     *
     * Time Complexity:
     * O(R² * P²)
     *
     * where:
     * - R = number of live ranges
     * - P = average number of program points
     */
    std::vector<Web> build(const std::vector<LiveRange>& liveRanges);

private:

    /**
     * @brief Checks whether a web contains a specific program line.
     *
     * @param web Target web.
     * @param line Program line number.
     *
     * @return True if the line exists in the web, false otherwise.
     *
     * Time Complexity:
     * O(P)
     *
     * where:
     * - P = number of program points
     */
    bool containsLine(const Web& web, int line) const;

    /**
     * @brief Determines whether a live range overlaps a web.
     *
     * Two structures overlap if they share at least one program point.
     *
     * @param web Existing web.
     * @param range Live range being tested.
     *
     * @return True if overlap exists, false otherwise.
     *
     * Time Complexity:
     * O(P²)
     */
    bool overlaps(const Web& web, const LiveRange& range) const;

    /**
     * @brief Merges a live range into an existing web.
     *
     * Duplicate program points are avoided during merging.
     *
     * @param web Target web.
     * @param range Live range to merge.
     *
     * Time Complexity:
     * O(P²)
     */
    void merge(Web& web, const LiveRange& range) const;
};

#endif //DA2_WEBBUILDER_H