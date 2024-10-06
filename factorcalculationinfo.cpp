#include "factorcalculationinfo.hpp"

void FactorCalculationInfo::calculateAndTime(void) {
   //start timer
    auto start = std::chrono::steady_clock::now();
    //calculate factorization
    factorization = primes::primeFactorization(n);
    //stop timer
    calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);

}

void FactorCalculationInfo::printPostCalcInfo(void) const {
    std::println("{}\n{}\n", factorization.asString(), calcTime);
}

