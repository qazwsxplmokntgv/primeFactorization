#include "factorization.hpp"

void Factorization::addNewFactor(unsigned long long base, uint_fast8_t exp) {
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
