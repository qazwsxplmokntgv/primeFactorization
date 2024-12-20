#pragma once

#include <cstdint>
#include <chrono>
#include <print>
#include "factorization.hpp"
#include "primes.hpp"

//used to store information on noteworthy factorizations for use in concluding statistical printouts
struct FactorCalculationInfo {
    FactorCalculationInfo(uint64_t n_) : n(n_) {} 

    //precondition: infoset.n is defined
    //postcondition: all fields of infoSet are correctly filled
    void calculateAndTime(void);

    //prints factorization and calcTime
    void printPostCalcInfo(void) const;

    uint64_t n;
    Factorization factorization;
    std::chrono::duration<long double, std::milli> calcTime;
};