#include "calculationinfo.hpp"

void FactorCalculationInfo::calculateAndTime(void) {
    auto start = std::chrono::steady_clock::now();
    factorization = primes::primeFactorization(n);
    calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);
}

void FactorCalculationInfo::printPostCalcInfo(void) const {
    std::println("{}\n{}\n", factorization.asString(), calcTime);
}

