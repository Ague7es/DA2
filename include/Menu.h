//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_MENU_H
#define DA2_MENU_H

#include <string>
#include <vector>
#include "LiveRange.h"
#include "AllocationConfig.h"
#include "Web.h"
#include "Graph.h"
#include "AllocationResult.h"

class Menu {
public:
    Menu();
    void run();

private:
    std::string rangesFile;
    std::string registersFile;
    std::string outputFile;
    std::vector<LiveRange> ranges;
    AllocationConfig config;
    std::vector<Web> webs;
    Graph<int> graph;
    AllocationResult result;

    bool hasInput = false;
    bool hasWebs = false;
    bool hasGraph = false;
    bool hasAllocation = false;

    void showMainMenu() const;
    void loadInputFiles();
    void showLiveRanges() const;
    void buildWebs();
    void showWebs() const;
    void buildInterferenceGraph();
    void showInterferenceGraph() const;
    void runRegisterAllocation();
    void showAllocationResult() const;
    void saveOutput() const;
};

#endif //DA2_MENU_H