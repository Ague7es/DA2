#include <iostream>
#include <string>

#include "../include/Menu.h"
#include "../include/RangeParser.h"
#include "../include/RegisterParser.h"
#include "../include/WebBuilder.h"
#include "../include/InterferenceGraphBuilder.h"
#include "../include/RegisterAllocator.h"
#include "../include/OutputWriter.h"

int main(int argc, char* argv[]) {
    try {
        if (argc == 5 && std::string(argv[1]) == "-b") {
            std::string rangesFile = argv[2];
            std::string registersFile = argv[3];
            std::string outputFile = argv[4];

            RangeParser rangeParser;
            RegisterParser registerParser;
            WebBuilder webBuilder;
            InterferenceGraphBuilder graphBuilder;
            RegisterAllocator allocator;

            auto ranges = rangeParser.parse(rangesFile);
            auto config = registerParser.parse(registersFile);
            auto webs = webBuilder.build(ranges);
            auto graph = graphBuilder.build(webs);
            auto result = allocator.allocateBasic(graph, webs, config.numberOfRegisters);

            std::cout << "Batch mode\n";
            std::cout << "Ranges file: " << rangesFile << "\n";
            std::cout << "Registers file: " << registersFile << "\n";
            std::cout << "Output file: " << outputFile << "\n\n";

            std::cout << "Parsed live ranges: " << ranges.size() << "\n";
            std::cout << "Registers: " << config.numberOfRegisters << "\n";
            std::cout << "Algorithm: " << config.algorithm << "\n";
            std::cout << "Parameter: " << config.parameter << "\n\n";

            std::cout << "Generated webs: " << webs.size() << "\n";

            for (const auto& web : webs) {
                std::cout << "web" << web.id
                          << " (" << web.variable << "): ";

                for (const auto& point : web.points) {
                    std::cout << point.line;

                    if (point.isStart) {
                        std::cout << "+";
                    }

                    if (point.isEnd) {
                        std::cout << "-";
                    }

                    std::cout << " ";
                }

                std::cout << "\n";
            }

            std::cout << "\nInterference graph:\n";

            for (auto vertex : graph.getVertexSet()) {
                std::cout << "web" << vertex->getInfo() << " -> ";

                for (auto edge : vertex->getAdj()) {
                    std::cout << "web" << edge->getDest()->getInfo() << " ";
                }

                std::cout << "\n";
            }

            std::cout << "\nRegister allocation result:\n";

            if (!result.success) {
                std::cout << "Allocation failed with "
                          << config.numberOfRegisters
                          << " register(s).\n";

                std::cout << "All webs assigned to memory.\n";

                for (int webId : result.spilledWebs) {
                    std::cout << "M: web" << webId << "\n";
                }
            }
            else {
                std::cout << "Registers used: "
                          << result.registersUsed
                          << "\n";

                for (const auto& pair : result.webToRegister) {
                    std::cout << "r"
                              << pair.second
                              << ": web"
                              << pair.first
                              << "\n";
                }
            }

            OutputWriter::write(outputFile, webs, result);

            return 0;
        }

        if (argc == 1) {
            Menu menu;
            menu.run();
            return 0;
        }

        std::cerr << "Usage:\n";
        std::cerr << "  " << argv[0] << "                    (interactive menu)\n";
        std::cerr << "  " << argv[0] << " -b ranges.txt registers.txt allocation.txt\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}