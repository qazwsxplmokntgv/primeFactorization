#include "timecategories.hpp"

void TimeCategories::increment(const long double timeMs) {
    //increments the appropriate counter for the calculation time category
    for (auto i = subdivisions.rbegin(); i != subdivisions.rend(); ++i) {
        if (timeMs >= i->floorMilliEquivalent) { 
            ++(i->count);
            break;
        }
    }
}

void TimeCategories::printout(void) const {
    //prints each counter, in 4 columns
    for (size_t i = 0; i < subdivisionCount / columnCount; ++i) 
        std::println("{:{}}{:{}}{:{}}{}", 
            //note that the divisions below will have to be altered if columnCount is changed from 4
            //each std::format corresponds to a column
            std::format("{}{}", subdivisions[i].displayText, subdivisions[i].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (subdivisionCount / 4)].displayText, subdivisions[i + (subdivisionCount / 4)].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (subdivisionCount / 2)].displayText, subdivisions[i + (subdivisionCount / 2)].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (3 * subdivisionCount / 4)].displayText, subdivisions[i + (3 * subdivisionCount / 4)].count));
}
