#include "sequence.hpp"

int main(void) {
    //TODO eliminate soft locks throughout main
    //gather preliminary information needed to begin sequence
    std::println("MODE: \n(1)Manual\n(2)Random");
    int selection;
    while (!(std::cin >> selection) || selection > 2 || selection < 1) {
        std::print("\033[A\33[2K\r");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    std::print("Count: ");
    unsigned long long count;
    while (!(std::cin >> count)) {
        std::print("\033[A\33[2K\rCount: ");
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    switch (selection) {
    case 1: //manual entry
        sequence::findUserRequestedFactorizations(count);
        break;
    case 2: //random surveying
        //gather additional necessary information
        std::print("Upper Bound (0 for max): ");
        unsigned long long maxN;
        while (!(std::cin >> maxN)) {
            std::print("\033[A\33[2K\rUpper Bound (0 for max): ");
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (maxN == 0) maxN = std::numeric_limits<unsigned long long>::max();

        std::print("Show individual results? (Y/n) ");
        char answer;
        do {
            while (!(std::cin >> answer)) {
                std::print("\033[A\33[2K\rShow individual results? (Y/n) ");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            answer = tolower(answer);
        } while (answer != 'y' && answer != 'n');

        std::println("\n");

        sequence::testRandomNumberFactorizations(count, answer == 'y', maxN);
        break;
    } 
    return 0;
}