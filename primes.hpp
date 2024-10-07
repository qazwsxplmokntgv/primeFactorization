#pragma once

#include <cmath>
#include <cstdint>
//#include <unordered_set>
#include "factorization.hpp"

namespace primes {
    //returns a map of prime factors of n and their respective powers in the form key == base, val == power
    Factorization primeFactorization(uint64_t n);

    inline bool isPrime(const uint64_t n);
    //if n is known to have no factors less than a certain number, that number can be passed in as the potentialFactorFloor
    inline bool isPrime(const uint64_t n, const unsigned long potentialPrimeFactorFloor);
} 
