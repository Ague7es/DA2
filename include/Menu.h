//
// Created by António Crispim Lopes dos Santos Anjos Guedes on 15/05/2026.
//

#ifndef DA2_MENU_H
#define DA2_MENU_H

#include <string>

class Menu {

public:
    Menu();
    void run();

private:
    std::string rangesFile;
    std::string registersFile;
    std::string outputFile;
    void showMainMenu() const;
    void loadInputFiles();
    void showCurrentFiles() const;
    void buildWebs();
    void buildInterferenceGraph();
    void runRegisterAllocation();
    void saveOutput();
};

#endif //DA2_MENU_H