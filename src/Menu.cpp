#include "../include/Menu.h"

#include "../include/RangeParser.h"
#include "../include/RegisterParser.h"
#include "../include/WebBuilder.h"
#include "../include/InterferenceGraphBuilder.h"
#include "../include/RegisterAllocator.h"
#include "../include/OutputWriter.h"

#include <iostream>
#include <limits>

Menu::Menu() = default;

void Menu::run() {
    int option;

    do {
        showMainMenu();
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid option.\n";
            continue;
        }

        switch (option) {
            case 1:
                loadInputFiles();
                break;
            case 2:
                showLiveRanges();
                break;
            case 3:
                buildWebs();
                break;
            case 4:
                showWebs();
                break;
            case 5:
                buildInterferenceGraph();
                break;
            case 6:
                showInterferenceGraph();
                break;
            case 7:
                runRegisterAllocation();
                break;
            case 8:
                showAllocationResult();
                break;
            case 9:
                saveOutput();
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cerr << "Invalid option.\n";
        }

    } while (option != 0);
}

void Menu::showMainMenu() const {
    std::cout << "\n========== DA Project 2 ==========\n";
    std::cout << "1. Load input files\n";
    std::cout << "2. Show live ranges\n";
    std::cout << "3. Build webs\n";
    std::cout << "4. Show webs\n";
    std::cout << "5. Build interference graph\n";
    std::cout << "6. Show interference graph\n";
    std::cout << "7. Run register allocation\n";
    std::cout << "8. Show allocation result\n";
    std::cout << "9. Save output\n";
    std::cout << "0. Exit\n";
    std::cout << "Option: ";
}

void Menu::loadInputFiles() {
    std::cout << "Ranges file path: ";
    std::cin >> rangesFile;

    std::cout << "Registers file path: ";
    std::cin >> registersFile;

    std::cout << "Output file path: ";
    std::cin >> outputFile;

    RangeParser rangeParser;
    RegisterParser registerParser;

    ranges = rangeParser.parse(rangesFile);
    config = registerParser.parse(registersFile);

    hasInput = true;
    hasWebs = false;
    hasGraph = false;
    hasAllocation = false;

    std::cout << "Input loaded successfully.\n";
    std::cout << "Live ranges: " << ranges.size() << "\n";
    std::cout << "Registers: " << config.numberOfRegisters << "\n";
    std::cout << "Algorithm: " << config.algorithm << "\n";
}

void Menu::showLiveRanges() const {
    if (!hasInput) {
        std::cerr << "Error: input files not loaded.\n";
        return;
    }

    for (const auto& range : ranges) {
        std::cout << range.variable << ": ";

        for (const auto& point : range.points) {
            std::cout << point.line;

            if (point.isStart) std::cout << "+";
            if (point.isEnd) std::cout << "-";

            std::cout << " ";
        }

        std::cout << "\n";
    }
}

void Menu::buildWebs() {
    if (!hasInput) {
        std::cerr << "Error: input files not loaded.\n";
        return;
    }

    WebBuilder webBuilder;
    webs = webBuilder.build(ranges);

    hasWebs = true;
    hasGraph = false;
    hasAllocation = false;

    std::cout << "Webs built successfully.\n";
    std::cout << "Generated webs: " << webs.size() << "\n";
}

void Menu::showWebs() const {
    if (!hasWebs) {
        std::cerr << "Error: webs not built.\n";
        return;
    }

    for (const auto& web : webs) {
        std::cout << "web" << web.id << " (" << web.variable << "): ";

        for (const auto& point : web.points) {
            std::cout << point.line;

            if (point.isStart) std::cout << "+";
            if (point.isEnd) std::cout << "-";

            std::cout << " ";
        }

        std::cout << "\n";
    }
}

void Menu::buildInterferenceGraph() {
    if (!hasWebs) {
        std::cerr << "Error: webs not built.\n";
        return;
    }

    InterferenceGraphBuilder graphBuilder;
    graph = graphBuilder.build(webs);

    hasGraph = true;
    hasAllocation = false;

    std::cout << "Interference graph built successfully.\n";
}

void Menu::showInterferenceGraph() const {
    if (!hasGraph) {
        std::cerr << "Error: interference graph not built.\n";
        return;
    }

    for (auto vertex : graph.getVertexSet()) {
        std::cout << "web" << vertex->getInfo() << " -> ";

        for (auto edge : vertex->getAdj()) {
            std::cout << "web" << edge->getDest()->getInfo() << " ";
        }

        std::cout << "\n";
    }
}

void Menu::runRegisterAllocation() {
    if (!hasGraph) {
        std::cerr << "Error: interference graph not built.\n";
        return;
    }

    RegisterAllocator allocator;

    if (config.algorithm == "basic") {
        result = allocator.allocateBasic(graph, webs, config.numberOfRegisters);
    }
    else {
        std::cerr << "Algorithm not implemented yet: "
                  << config.algorithm << "\n";
        return;
    }

    hasAllocation = true;

    std::cout << "Register allocation completed.\n";

    if (!result.success) {
        std::cerr << "Warning: assignment to the provided number of registers was not possible.\n";
    }
}

void Menu::showAllocationResult() const {
    if (!hasAllocation) {
        std::cerr << "Error: allocation not executed.\n";
        return;
    }

    if (!result.success) {
        std::cout << "Allocation failed.\n";
        std::cout << "All webs assigned to memory.\n";

        for (int webId : result.spilledWebs) {
            std::cout << "M: web" << webId << "\n";
        }

        return;
    }

    std::cout << "Registers used: " << result.registersUsed << "\n";

    for (const auto& pair : result.webToRegister) {
        std::cout << "r" << pair.second << ": web" << pair.first << "\n";
    }
}

void Menu::saveOutput() const {
    if (!hasAllocation) {
        std::cerr << "Error: allocation not executed.\n";
        return;
    }

    OutputWriter::write(outputFile, webs, result);

    std::cout << "Output written to: " << outputFile << "\n";
}
