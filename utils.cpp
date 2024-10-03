#include "utils.hpp"

void timeCategories::increment(const long double timeMs) {
    //increments the appropriate counter for the calculation time category
    for (auto i = subdivisions.rbegin(); i != subdivisions.rend(); ++i) {
        if (timeMs >= i->floorMilliEquivalent) { 
            ++i->count;
            break;
        }
    }
}

void timeCategories::printout(void) const {
    printDivider("Counts (fastest applicable category only)");
    //prints each counter, in two columns
    for (size_t i = 0; i < subdivisionCount / 2; ++i) 
        std::cout << std::format("{:{}}{}\n", 
            std::format("{}{}", subdivisions[i].displayText, subdivisions[i].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (subdivisionCount / 2)].displayText, subdivisions[i + (subdivisionCount / 2)].count));
}

void printDivider(const std::string&& leftHeader, const std::string&& rightHeader) {
    static constexpr size_t indent = 3;
    std::cout << std::format("\033[1;4;97;53m{:-<{}}{:-<{}}{:-<{}}\033[0m\n", "", indent, leftHeader, panelWidth, rightHeader, panelWidth - indent);
}
