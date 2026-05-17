//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_PROGRAMPOINT_H
#define DA2_PROGRAMPOINT_H



/**
 * @struct ProgramPoint
 *
 * @brief Represents a program point inside a live range or web.
 *
 * Each program point corresponds to a specific line in the program and may
 * indicate:
 * - the beginning of a live range;
 * - the end of a live range;
 * - or an intermediate point.
 */
struct ProgramPoint {
    int line;
    bool isStart;
    bool isEnd;

    /**

     * @brief Constructs a program point.
     *
     * @param line Program line number.
     * @param isStart True if the point starts a live range.
     * @param isEnd True if the point ends a live range.
     *
     * Time Complexity:
     * O(1)
     */
    ProgramPoint(int line, bool isStart = false, bool isEnd = false): line(line), isStart(isStart), isEnd(isEnd) {}
};

#endif //DA2_PROGRAMPOINT_H