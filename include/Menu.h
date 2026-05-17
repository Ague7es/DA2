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

/**
 * @class Menu
 *
 * @brief Provides the interactive command-line interface for the application.
 *
 * The menu allows the user to:
 * - load input files;
 * - visualize live ranges and webs;
 * - build the interference graph;
 * - execute register allocation algorithms;
 * - visualize and save allocation results.
 */
class Menu {
public:

    /**
    * @brief Constructs the application menu.
    */
    Menu();

    /**
     * @brief Starts the interactive menu loop.
     *
     * Handles user input and dispatches the selected operations.
     *
     * Time Complexity:
     * Depends on the selected menu operations.
     */
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

    /**
     * @brief Displays the main menu.
     *
     * Time Complexity:
     * O(1)
     */
    void showMainMenu() const;

    /**
     * @brief Loads and parses input files.
     *
     * Time Complexity:
     * Depends on parser complexity.
     */
    void loadInputFiles();

    /**
     * @brief Displays all parsed live ranges.
     *
     * Time Complexity:
     * O(R * P)
     *
     * where:
     * - R = number of live ranges
     * - P = average number of program points
     */
    void showLiveRanges() const;

    /**
     * @brief Builds webs from live ranges.
     *
     * Time Complexity:
     * Depends on WebBuilder implementation.
     */
    void buildWebs();

    /**
     * @brief Displays generated webs.
     *
     * Time Complexity:
     * O(W * P)
     *
     * where:
     * - W = number of webs
     * - P = average number of program points
     */
    void showWebs() const;

    /**
     * @brief Builds the interference graph.
     *
     * Time Complexity:
     * Depends on InterferenceGraphBuilder implementation.
     */
    void buildInterferenceGraph();

    /**
     * @brief Displays the interference graph adjacency lists.
     *
     * Time Complexity:
     * O(V + E)
     *
     * where:
     * - V = number of vertices
     * - E = number of edges
     */
    void showInterferenceGraph() const;

    /**
     * @brief Executes the selected register allocation algorithm.
     *
     * Time Complexity:
     * Depends on the chosen allocation algorithm.
     */
    void runRegisterAllocation();

    /**
     * @brief Displays the allocation result.
     *
     * Time Complexity:
     * O(W)
     *
     * where:
     * - W = number of webs
     */
    void showAllocationResult() const;

    /**
     * @brief Writes the allocation result to the output file.
     *
     * Time Complexity:
     * Depends on OutputWriter implementation.
     */
    void saveOutput() const;
};

#endif //DA2_MENU_H