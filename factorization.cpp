#include "factorization.hpp"

Factorization::Factorization() {
    //covers the vast majority of factorizations, including the slow case where the 2nd largest factor is very large
    //in worst case, requires no more than 1 reallocation (1.5x scaling assumed; still also applies to 2x)
    //note that lengths above 10 are already very rare as well
    factors.reserve(11);
}

void Factorization::addNewFactor(const unsigned long long base, const uint_fast8_t exp) {
    factorCount += exp; //add to count of total factors
    factors.emplace_back(base, exp); //append to the end of the list
}

std::string Factorization::asString() const {
    if (factors.empty()) //if no factors, prime factorization DNE
        return "= DNE";
    else {
        std::string out("="); 
        for (const auto& fac : factors) {
            //append each factor's base
            out += std::format(" {}", fac.base);
            //if this factor's exponent is greater than 1, denote with ^exp
            //uint_fast8_t is usually an unsigned char, hence the need for a cast
            if (fac.exp > 1) out += std::format("^{}", (unsigned short)fac.exp);
        }
        return out;
    }
}

const uint_fast8_t Factorization::getFactorCount() const {
    return factorCount;
}

const uint_fast8_t Factorization::getUniqueFactorCount() const {
    return factors.size();
}
