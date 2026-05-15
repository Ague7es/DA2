#include <iostream>
#include <string>

#include "../include/Menu.h"

int main(int argc, char* argv[]) {
    try {
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