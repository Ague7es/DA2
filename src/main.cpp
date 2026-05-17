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
            AllocationResult result;

            auto ranges = rangeParser.parse(rangesFile);
            auto config = registerParser.parse(registersFile);
            auto webs = webBuilder.build(ranges);
            auto graph = graphBuilder.build(webs);

            if (config.algorithm == "basic") {
                result = allocator.allocateBasic(graph, webs, config.numberOfRegisters);
            }
            else if (config.algorithm == "spilling") {
                result = allocator.allocateSpilling(graph, webs, config.numberOfRegisters, config.parameter);
            }
            else if (config.algorithm == "splitting") {
                result = allocator.allocateWithSplitting(webs, config.numberOfRegisters, config.parameter);
                if (!result.finalWebs.empty()) webs = result.finalWebs;
            }
            else if (config.algorithm == "free") {
                result = allocator.allocateFree(graph, webs, config.numberOfRegisters);
            }
            else {
                std::cerr << "Algorithm not implemented yet: " << config.algorithm << "\n";
                return 1;
            }

            OutputWriter::write(outputFile, webs, result);

            if (!result.success) {

                std::cerr << "Warning: assignment to the provided number "
                          << "of registers was not possible.\n";
            }

            std::cout << "Processing completed successfully.\n";
            std::cout << "Output written to: " << outputFile << "\n";
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