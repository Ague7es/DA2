//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/OutputWriter.h"

#include <fstream>
#include <stdexcept>
#include <algorithm>

void OutputWriter::write(const std::string& filename, const std::vector<Web>& webs, const AllocationResult& result) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    std::vector<Web> sortedWebs = webs;

    std::sort(sortedWebs.begin(), sortedWebs.end(),[](const Web& a, const Web& b) { return a.id < b.id;});

    file << "# Total number of webs followed by the listing of the program points of each one\n";
    file << "# program points in each web are sorted in ascending order\n";
    file << "webs: " << sortedWebs.size() << "\n";

    for (const auto& web : sortedWebs) {
        writeWeb(file, web);
    }

    file << "# Total number of registers used, followed by assignment to webs\n";
    file << "registers: " << result.registersUsed << "\n";

    if (!result.success) {
        for (const auto& web : sortedWebs) {
            file << "M: web" << web.id << "\n";
        }
        file.flush();
        file.close();
        return;
    }

    for (const auto& pair : result.webToRegister) {
        int webId = pair.first;
        int registerId = pair.second;

        file << "r" << registerId << ": web" << webId << "\n";

    }
    file.flush();
    file.close();
}

void OutputWriter::writeWeb(std::ofstream& file, const Web& web) {
    file << "web" << web.id << ": ";

    for (size_t i = 0; i < web.points.size(); i++) {
        const auto& point = web.points[i];

        file << point.line;

        if (point.isStart) {
            file << "+";
        }

        if (point.isEnd) {
            file << "-";
        }

        if (i + 1 < web.points.size()) {
            file << ",";
        }
    }

    file << "\n";
}