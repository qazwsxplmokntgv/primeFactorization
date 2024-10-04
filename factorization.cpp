#include "factorization.hpp"

Factorization::Factorization() {
    //covers the vast majority of factorizations, including the slow case where the 2nd largest factor is very large
    //in worst case, requires no more than 1 reallocation
    factors.reserve(11);
}

void Factorization::addNewFactor(const unsigned long long base, const uint_fast8_t exp) {
    factorCount += exp;
    factors.emplace_back(base, exp);
}

std::string Factorization::asString() const {
    if (factors.empty()) 
        return "= DNE";
    else {
        std::string out("="); 
        for (const auto& fac : factors) {
            out += std::format(" {}", fac.base);
            if (fac.exp > 1) out += std::format("^{}", (unsigned short)fac.exp);
        }
        return out;
    }
}

const int Factorization::getFactorCount() const {
    return factorCount;
}

const int Factorization::getUniqueFactorCount() const {
    return factors.size();
}
