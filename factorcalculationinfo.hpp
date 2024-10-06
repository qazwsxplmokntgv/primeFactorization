#pragma once

#include <chrono>
#include <print>
#include "factorization.hpp"
#include "primes.hpp"

//used to store information on noteworthy factorizations for use in concluding statistical printouts
struct FactorCalculationInfo {
    FactorCalculationInfo(unsigned long long n_ = 0) : n(n_) {} 

    //precondition: infoset.n has the correct value value
    //postcondition: all fields of infoSet are correctly filled
    void calculateAndTime(void);

    //prints factorization and calcTime
    void printPostCalcInfo(void) const;

    unsigned long long n;
    Factorization factorization;
    std::chrono::duration<long double, std::milli> calcTime;
};