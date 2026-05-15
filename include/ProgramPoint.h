//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_PROGRAMPOINT_H
#define DA2_PROGRAMPOINT_H

struct ProgramPoint {
    int line;
    bool isStart;
    bool isEnd;

    ProgramPoint(int line, bool isStart = false, bool isEnd = false): line(line), isStart(isStart), isEnd(isEnd) {}
};

#endif //DA2_PROGRAMPOINT_H