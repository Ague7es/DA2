#include <iostream>
#include <string>

#include "../include/Menu.h"
#include "../include/RangeParser.h"
#include "../include/RegisterParser.h"

int main(int argc, char* argv[]) {
    try {
        if (argc == 5 && std::string(argv[1]) == "-b") {
            std::string rangesFile = argv[2];
            std::string registersFile = argv[3];
            std::string outputFile = argv[4];

            RangeParser rangeParser;
            RegisterParser registerParser;

            auto ranges = rangeParser.parse(rangesFile);
            auto config = registerParser.parse(registersFile);

            std::cout << "Batch mode\n";
            std::cout << "Ranges file: " << rangesFile << "\n";
            std::cout << "Registers file: " << registersFile << "\n";
            std::cout << "Output file: " << outputFile << "\n\n";

            std::cout << "Parsed live ranges: " << ranges.size() << "\n";
            std::cout << "Registers: " << config.numberOfRegisters << "\n";
            std::cout << "Algorithm: " << config.algorithm << "\n";
            std::cout << "Parameter: " << config.parameter << "\n";

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