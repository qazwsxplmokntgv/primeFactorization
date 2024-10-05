#pragma once

#include <chrono>
#include <iostream>
#include <random>
#include <print>
#include "statset.hpp"
#include "primes.hpp"
#include "factorization.hpp"

namespace sequence {
    //allows the user to input count numbers of their choice in the range of long max, and prints 
    //a. the factorizations of each, and 
    //b. the time taken to calculate the above
    void findUserRequestedFactorizations(const unsigned long long count);

    //randomly generates several numbers and processes them as findUserRequestedFactorizations() does. 
    //prints several statistics upon conclusion
    void testRandomNumberFactorizations(const unsigned long long count, const bool shouldReportEachFactorization, const unsigned long long maxN);

} 