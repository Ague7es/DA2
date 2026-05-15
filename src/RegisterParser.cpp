//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/RegisterParser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

AllocationConfig RegisterParser::parse(const std::string& filename) const {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open registers file: " + filename);
    }

    AllocationConfig config;
    bool foundRegisters = false;
    bool foundAlgorithm = false;

    std::string line;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t colonPos = line.find(':');

        if (colonPos == std::string::npos) {
            throw std::runtime_error("Invalid config line: " + line);
        }

        std::string key = trim(line.substr(0, colonPos));
        std::string value = trim(line.substr(colonPos + 1));

        if (key == "registers") {
            config.numberOfRegisters = std::stoi(value);
            foundRegisters = true;

            if (config.numberOfRegisters <= 0) {
                throw std::runtime_error("Number of registers must be positive.");
            }
        }
        else if (key == "algorithm") {
            size_t commaPos = value.find(',');

            if (commaPos == std::string::npos) {
                config.algorithm = trim(value);
                config.parameter = 0;
            }
            else {
                config.algorithm = trim(value.substr(0, commaPos));
                config.parameter = std::stoi(trim(value.substr(commaPos + 1)));
            }

            if (config.algorithm != "basic" &&
                config.algorithm != "spilling" &&
                config.algorithm != "splitting" &&
                config.algorithm != "free") {
                throw std::runtime_error("Unknown algorithm: " + config.algorithm);
            }

            if ((config.algorithm == "spilling" || config.algorithm == "splitting") &&
                config.parameter <= 0) {
                throw std::runtime_error("Spilling/splitting algorithm requires positive parameter.");
            }

            foundAlgorithm = true;
        }
    }

    if (!foundRegisters) {
        throw std::runtime_error("Missing registers field.");
    }

    if (!foundAlgorithm) {
        throw std::runtime_error("Missing algorithm field.");
    }

    return config;
}

std::string RegisterParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, last - first + 1);
}