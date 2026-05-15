//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/RangeParser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

std::vector<LiveRange> RangeParser::parse(const std::string& filename) const {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open ranges file: " + filename);
    }

    std::vector<LiveRange> ranges;
    std::string line;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t colonPos = line.find(':');

        if (colonPos == std::string::npos) {
            throw std::runtime_error("Invalid range line: " + line);
        }

        std::string variable = trim(line.substr(0, colonPos));
        std::string pointsText = trim(line.substr(colonPos + 1));

        if (variable.empty() || pointsText.empty()) {
            throw std::runtime_error("Invalid range line: " + line);
        }

        LiveRange range;
        range.variable = variable;

        std::stringstream ss(pointsText);
        std::string token;

        while (std::getline(ss, token, ',')) {
            token = trim(token);

            if (!token.empty()) {
                range.points.push_back(parsePoint(token));
            }
        }

        if (range.points.empty()) {
            throw std::runtime_error("Live range without points: " + line);
        }

        ranges.push_back(range);
    }

    return ranges;
}

std::string RangeParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, last - first + 1);
}

ProgramPoint RangeParser::parsePoint(const std::string& token) {
    bool isStart = false;
    bool isEnd = false;

    std::string numberText = token;

    if (!token.empty() && token.back() == '+') {
        isStart = true;
        numberText = token.substr(0, token.size() - 1);
    }
    else if (!token.empty() && token.back() == '-') {
        isEnd = true;
        numberText = token.substr(0, token.size() - 1);
    }

    if (numberText.empty()) {
        throw std::runtime_error("Invalid program point: " + token);
    }

    for (char c : numberText) {
        if (!std::isdigit(c)) {
            throw std::runtime_error("Invalid program point: " + token);
        }
    }

    int lineNumber = std::stoi(numberText);

    return ProgramPoint(lineNumber, isStart, isEnd);
}