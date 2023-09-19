#include "loadedDice.h"
#include <iostream>

int LoadedDice::roll() {
    std::cout << "Input a roll between 2 and 12:" << std::endl;
    while (true) {
        std::cout << "> ";
        int num;
        if (std::cin >> num && num >= 2 && num <= 12) {
            return num;
        } else {
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
            std::cin.clear();
            std::cin.ignore();
        }
    }
}
