#include "factorization.hpp"

Factorization::Factorization() {
    //covers the vast majority of factorizations, including the slow case where the 2nd largest factor is very large
    //in worst case, requires no more than 1 reallocation (1.5x scaling assumed; still also applies to 2x)
    //note that lengths above 11 are very rare also
    factors.reserve(11);
}

void Factorization::addNewFactor(const base_t base, const exp_t exp) {
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
            //caret notation is redundant when exp <= 1
            //uint_fast8_t is often defined as an unsigned char, hence the need for a cast
            if (fac.exp > 1) out += std::format("^{}", static_cast<unsigned short>(fac.exp));
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

const Factorization::container_t& Factorization::viewFactors(void) const {
    return factors;
}
