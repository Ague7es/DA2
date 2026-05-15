//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#include "../include/Menu.h"

#include <iostream>
#include <limits>

Menu::Menu()
    : rangesFile(""),
      registersFile(""),
      outputFile("") {}

void Menu::run() {
    int option;

    do {
        showMainMenu();
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid option. Please insert a number.\n";
            continue;
        }

        switch (option) {
            case 1:
                loadInputFiles();
                break;
            case 2:
                showCurrentFiles();
                break;
            case 3:
                buildWebs();
                break;
            case 4:
                buildInterferenceGraph();
                break;
            case 5:
                runRegisterAllocation();
                break;
            case 6:
                saveOutput();
                break;
            case 0:
                std::cout << "Exiting program...\n";
                break;
            default:
                std::cerr << "Invalid option. Try again.\n";
        }

    } while (option != 0);
}

void Menu::showMainMenu() const {
    std::cout << " Register Allocation Tool\n";
    std::cout << "==================================\n";
    std::cout << "1. Load input files\n";
    std::cout << "2. Show current files\n";
    std::cout << "3. Build webs\n";
    std::cout << "4. Build interference graph\n";
    std::cout << "5. Run register allocation\n";
    std::cout << "6. Save output\n";
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

    std::cout << "Files loaded successfully.\n";
}

void Menu::showCurrentFiles() const {
    std::cout << "\nCurrent files:\n";

    std::cout << "Ranges file: ";
    std::cout << (rangesFile.empty() ? "not set" : rangesFile) << "\n";

    std::cout << "Registers file: ";
    std::cout << (registersFile.empty() ? "not set" : registersFile) << "\n";

    std::cout << "Output file: ";
    std::cout << (outputFile.empty() ? "not set" : outputFile) << "\n";
}

void Menu::buildWebs() {
    if (rangesFile.empty()) {
        std::cerr << "Error: ranges file not loaded.\n";
        return;
    }

    std::cout << "TODO: parse ranges file and build webs.\n";
}

void Menu::buildInterferenceGraph() {
    std::cout << "TODO: build interference graph from webs.\n";
}

void Menu::runRegisterAllocation() {
    if (registersFile.empty()) {
        std::cerr << "Error: registers file not loaded.\n";
        return;
    }

    std::cout << "TODO: run graph coloring register allocation.\n";
}

void Menu::saveOutput() {
    if (outputFile.empty()) {
        std::cerr << "Error: output file not set.\n";
        return;
    }

    std::cout << "TODO: save allocation result to output file.\n";
}