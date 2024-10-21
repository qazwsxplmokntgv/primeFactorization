#pragma once

#include <cmath>
#include <numeric>
#include <cstdint>
#include <unordered_set>
#include "factorization.hpp"

//notes: isPrime(uint64_t, uint64_t) possibly use uint32_t for i? careful about overflow

namespace primes {
    //returns a map of prime factors of n and their respective powers in the form key == base, val == power
    Factorization primeFactorization(uint64_t n);

    inline bool isPrime(const uint64_t n);
    //if n is known to have no factors less than a certain number, that number can be passed in as the potentialFactorFloor
    inline bool isPrime(const uint64_t n, const uint64_t potentialFactorFloor);

    std::unordered_set<uint32_t> populatePrimeSet();
} 
