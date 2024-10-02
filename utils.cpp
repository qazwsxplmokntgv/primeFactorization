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

std::string toString(const std::vector<factor>& factorization) {
    if (factorization.empty()) [[unlikely]]
        return " DNE";
    else {
        std::string out("="); // intentionally == in practice
        for (const auto& factor : factorization) {
            out += std::format(" {}", factor.base);
            if (factor.exp > 1) out += std::format("^{}", (unsigned short)factor.exp);
        }
        return out;
    }
}

void rankIfApplicable(const factorizedNumInfo& newItem, std::vector<factorizedNumInfo>& existingRankings, const std::function<bool(const factorizedNumInfo&, const factorizedNumInfo&)>&& comparison) {
    //for each stored value, 
    for (size_t i = 0; i < existingRankings.size(); ++i) { 
        //find if the new factorization outranks any existing ranked factorizations
        if (comparison(newItem, existingRankings[i])) {
            //shifts indexes at or after i to the right, discarding the lowest ranked value
            for (size_t j = existingRankings.size() - 1; j > i; --j) {
                existingRankings[j] = existingRankings[j - 1];
            }
            //places the new factorization into the opened slot
            existingRankings[i] = newItem;
            break;
        }
    }
}

unsigned int getFactorCount(const std::vector<factor>& factorization) {
    unsigned int count = 0;
    for (const factor& fac : factorization) count += fac.exp;
    return count;
}
