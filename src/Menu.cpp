#include "../include/Menu.h"

#include "../include/RangeParser.h"
#include "../include/RegisterParser.h"
#include "../include/WebBuilder.h"
#include "../include/InterferenceGraphBuilder.h"
#include "../include/RegisterAllocator.h"
#include "../include/OutputWriter.h"
#include "../include/AllocationResult.h"

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
            std::cout << "Invalid option.\n";
            option = -1;
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
                std::cout << "Invalid option.\n";
        }

    } while (option != 0);
}

void Menu::showMainMenu() const {
    std::cout << "\n========== Register Allocation Tool==========\n";
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
    std::string rangesName;
    std::string registersName;
    std::string outputName;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nRanges file name (empty to cancel): ";
    std::getline(std::cin, rangesName);

    if (rangesName.empty()) {
        std::cout << "Load cancelled.\n";
        return;
    }

    std::cout << "Registers file name (empty to cancel): ";
    std::getline(std::cin, registersName);

    if (registersName.empty()) {
        std::cout << "Load cancelled.\n";
        return;
    }

    std::cout << "Output file name (empty to cancel): ";
    std::getline(std::cin, outputName);

    if (outputName.empty()) {
        std::cout << "Load cancelled.\n";
        return;
    }

    std::string newRangesFile = "../dataset/ranges/" + rangesName;
    std::string newRegistersFile = "../dataset/registers/" + registersName;
    std::string newOutputFile = "../dataset/output/" + outputName;

    try {
        RangeParser rangeParser;
        RegisterParser registerParser;

        auto newRanges = rangeParser.parse(newRangesFile);
        auto newConfig = registerParser.parse(newRegistersFile);

        rangesFile = newRangesFile;
        registersFile = newRegistersFile;
        outputFile = newOutputFile;

        ranges = newRanges;
        config = newConfig;

        webs.clear();
        result = AllocationResult();

        hasInput = true;
        hasWebs = false;
        hasGraph = false;
        hasAllocation = false;

        std::cout << "\nInput loaded successfully.\n";
        std::cout << "Live ranges: "
                  << ranges.size()
                  << "\n";

        std::cout << "Registers: "
                  << config.numberOfRegisters
                  << "\n";

        std::cout << "Algorithm: "
                  << config.algorithm
                  << "\n";
    }
    catch (const std::exception& e) {

        std::cout << "\nError: "
                  << e.what()
                  << "\n";

        std::cout << "Input was not loaded. "
                  << "Returning to menu.\n";

        hasInput = false;
        hasWebs = false;
        hasGraph = false;
        hasAllocation = false;
    }
}

void Menu::showLiveRanges() const {
    if (!hasInput) {
        std::cout << "Error: input files not loaded.\n";
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
        std::cout << "Error: input files not loaded.\n";
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
        std::cout << "Error: webs not built.\n";
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
        std::cout << "Error: webs not built.\n";
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
        std::cout << "Error: interference graph not built.\n";
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
        std::cout << "Error: interference graph not built.\n";
        return;
    }

    RegisterAllocator allocator;

    if (config.algorithm == "basic") {
        result = allocator.allocateBasic(graph, webs, config.numberOfRegisters);
    }
    else if (config.algorithm == "spilling") {
        result = allocator.allocateSpilling(graph, webs, config.numberOfRegisters, config.parameter);
    }
    else if (config.algorithm == "splitting") {
        result = allocator.allocateWithSplitting(webs, config.numberOfRegisters, config.parameter);
        // splitting may produce derived webs — update the menu's web list
        if (!result.finalWebs.empty()) webs = result.finalWebs;
    }
    else if (config.algorithm == "free") {
        result = allocator.allocateFree(graph, webs, config.numberOfRegisters);
    }
    else {
        std::cout << "Algorithm not implemented yet: "
                  << config.algorithm << "\n";
        return;
    }

    hasAllocation = true;
    if (!result.success) {
        std::cout << "\nRegister allocation failed.\n";
        std::cout << "Assignment to the provided number " << "of registers was not possible.\n";
    }
    else {
        std::cout << "\nRegister allocation completed successfully.\n";
    }
}

void Menu::showAllocationResult() const {
    if (!hasAllocation) {
        std::cout << "Error: allocation not executed.\n";
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
        std::cout << "\nError: allocation not executed.\n";
        return;
    }

    try {
        OutputWriter::write(outputFile, webs, result);
        std::cout << "\nOutput written to: " << outputFile << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "\nError: " << e.what() << "\n";
        std::cout << "Output was not saved. Returning to menu.\n";
    }
}
