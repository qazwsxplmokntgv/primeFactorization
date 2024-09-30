#include "utils.hpp"

void timeCategories::increment(const long double timeMs) {
    //increments the appropriate counter for the calculation time category
    //reverse this order with different conditions is likely better in performance due to nature of data
    //however this is much more readable 
    //TODO find compromise on that
    if (timeMs >= 18000000) ++thirtyMinOrMore;
    else if (timeMs >= 6000000) ++tenMinOrMore;
    else if (timeMs >= 3000000) ++fiveMinOrMore;
    else if (timeMs >= 60000) ++minOrMore;
    else if (timeMs >= 30000) ++thirtySecOrMore;
    else if (timeMs >= 10000) ++tenSecOrMore;
    else if (timeMs >= 5000) ++fiveSecOrMore;
    else if (timeMs >= 3000) ++threeSecOrMore;
    else if (timeMs >= 1000) ++secOrMore;
    else if (timeMs >= 500) ++halfSecOrMore;
    else if (timeMs >= 250) ++quarterSecOrMore;
    else if (timeMs >= 1) ++milliOrMore;
    else if (timeMs >= .5) ++halfMilliOrMore;
    else if (timeMs >= .25) ++quarterMilliOrMore;
    else if (timeMs >= .0001) ++microOrMore;
    else ++subMicro;

}

void timeCategories::print(void) const {
    //prints each counter, in two columns
    std::cout << "\033[1;4;97m---Counts (fastest applicable category only)------------------------------------------------------------------------------------\033[0m\n"
        << "<   1  μs: " << std::setw(13) << subMicro           << ">=  3  sec: " << threeSecOrMore  << '\n'        
        << ">=  1  μs: " << std::setw(13) << microOrMore        << ">=  5  sec: " << fiveSecOrMore   << '\n'      
        << ">=  ¼  ms: " << std::setw(13) << quarterMilliOrMore << ">= 10  sec: " << tenSecOrMore    << '\n'        
        << ">=  ½  ms: " << std::setw(13) << halfMilliOrMore    << ">= 30  sec: " << thirtySecOrMore << '\n'     
        << ">=  1  ms: " << std::setw(13) << milliOrMore        << ">=  1  min: " << minOrMore       << '\n'        
        << ">=  ¼ sec: " << std::setw(13) << quarterSecOrMore   << ">=  5  min: " << fiveMinOrMore   << '\n'      
        << ">=  ½ sec: " << std::setw(13) << halfSecOrMore      << ">= 10  min: " << tenMinOrMore    << '\n'        
        << ">=  1 sec: " << std::setw(13) << secOrMore          << ">= 30  min: " << thirtyMinOrMore << "\n\n";      
}

void printFactorization(const std::vector<factor>& factorization, std::ostream& stream) {
    if (factorization.empty()) [[unlikely]] {
        stream << " DNE";
    } 
    else {
        stream << '=';
        for (const auto& factor : factorization) {
            stream << ' ' << factor.base;
            if (factor.exp > 1) stream << '^' << (int)factor.exp;
        }
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
