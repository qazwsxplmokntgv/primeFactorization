#include "timecategories.hpp"

void TimeCategories::incrementAppropriateCategory(const long double timeMs) {
    for (subdivision& sub : subdivisions) {
        if (timeMs < sub.milliEquiv) { 
            ++sub.count;
            break;
        }
    }
}

void TimeCategories::printout(FILE* outStream) const {
    //prints each counter, in 4 columns
    for (size_t i = 0; i < subdivisionCount / columnCount; ++i) 
        std::println(outStream, "{:{}}{:{}}{:{}}{}", 
            //note that the divisions below will have to be altered if columnCount is changed from 4
            //each std::format corresponds to a column
            std::format("{}{}", subdivisions[i].displayText, subdivisions[i].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (subdivisionCount / 4)].displayText, subdivisions[i + (subdivisionCount / 4)].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (subdivisionCount / 2)].displayText, subdivisions[i + (subdivisionCount / 2)].count), miniPanelWidth, 
            std::format("{}{}", subdivisions[i + (3 * subdivisionCount / 4)].displayText, subdivisions[i + (3 * subdivisionCount / 4)].count));
}
