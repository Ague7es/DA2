//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/OutputWriter.h"

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <map>

void OutputWriter::write(const std::string& filename, const std::vector<Web>& webs, const AllocationResult& result) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    std::vector<Web> sortedWebs = webs;

    std::sort(sortedWebs.begin(), sortedWebs.end(),[](const Web& a, const Web& b) {
                  if (a.variable != b.variable) {
                      return a.variable < b.variable;
                  }

                  int firstA = a.points.empty() ? 0 : a.points.front().line;
                  int firstB = b.points.empty() ? 0 : b.points.front().line;

                  return firstA < firstB;
              });

    std::map<int, int> oldToNewId;

    for (size_t i = 0; i < sortedWebs.size(); i++) {
        oldToNewId[sortedWebs[i].id] = static_cast<int>(i);
    }

    file << "# Total number of webs followed by the listing of the program points of each one\n";
    file << "# program points in each web are sorted in ascending order\n";

    file << "webs: " << sortedWebs.size() << "\n";

    for (size_t i = 0; i < sortedWebs.size(); i++) {
        writeWeb(file, sortedWebs[i], static_cast<int>(i));
    }

    file << "# Total number of registers used, followed by assignment to webs\n";
    file << "registers: " << result.registersUsed << "\n";

    if (!result.success) {
        for (size_t i = 0; i < sortedWebs.size(); i++) {
            file << "M: web" << i << "\n";
        }

        file.flush();
        file.close();
        return;
    }

    std::vector<std::pair<int, int>> assignments;

    for (const auto& pair : result.webToRegister) {
        int oldWebId = pair.first;
        int registerId = pair.second;
        int newWebId = oldToNewId[oldWebId];

        assignments.push_back({newWebId, registerId});
    }

    std::sort(assignments.begin(), assignments.end());

    for (const auto& assignment : assignments) {
        int newWebId = assignment.first;
        int registerId = assignment.second;

        file << "r" << registerId
             << ": web" << newWebId
             << "\n";
    }

    file.flush();
    file.close();
}

void OutputWriter::writeWeb(std::ofstream& file, const Web& web, int printedId) {
    file << "web" << printedId << ": ";

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