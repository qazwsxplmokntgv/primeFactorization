#include "factorization.hpp"

Factorization::Factorization() {
    //easily sufficient for most cases, with at most 1 reallocation required to fit larger factorizations (capped at 16 with 64 bit unsigned ints)
    factors.reserve(11);
}

void Factorization::addNewFactor(const unsigned long long base, const uint_fast8_t exp) {
    factors.emplace_back(base, exp);
    factorCount += exp;
}

std::string Factorization::asString() const {
    if (factors.empty()) [[unlikely]]
        return "= DNE";
    else {
        std::string out("="); // intentionally == in practice
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
