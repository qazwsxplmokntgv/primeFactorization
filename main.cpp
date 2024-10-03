#include "sequence.hpp"

int main(void) {
    //TODO eliminate soft locks throughout main
    //gather preliminary information needed to begin sequence
    std::println("MODE: \n(1)Manual\n(2)Random");
    int selection = 0;
    while (selection > 2 || selection < 1) std::cin >> selection;
    
    std::print("Count: ");
    unsigned long long count;
    while (!(std::cin >> count)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    switch (selection) {
    case 1: //manual entry
        sequence::findUserRequestedFactorizations(count);
        break;
    case 2: //random surveying
        //gather additional necessary information
        unsigned long long maxN;
        std::print("Upper Bound (0 for max): ");
        while (!(std::cin >> maxN)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (maxN == 0) maxN = std::numeric_limits<unsigned long long>::max();

        char answer;
        std::print("Show individual results? (Y/n) ");
        do {
            std::cin >> answer;
            answer = tolower(answer);
        } while (answer != 'y' && answer != 'n');
        std::println("\n");

        sequence::testRandomNumberFactorizations(count, answer == 'y', maxN);
        break;
    } 
    return 0;
}