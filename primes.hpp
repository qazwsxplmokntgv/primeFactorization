#pragma once

#include <cmath>
#include <cstdint>
//#include <unordered_set>
#include "factorization.hpp"

namespace primes {
    //returns a map of prime factors of n and their respective powers in the form key == base, val == power
    Factorization primeFactorization(unsigned long long n);

    //returns whether n is prime or not
    inline bool isPrime(const unsigned long long n);
    //if n is known to have no factors less than a certain number, that number can be passed in as the potentialFactorFloor
    inline bool isPrime(const unsigned long long n, const unsigned long long potentialPrimeFactorFloor);

    // inline bool isPrimeRemembers(const unsigned long long n);
    // inline bool isPrimeRemembers(const unsigned long long n, const unsigned long long potentialPrimeFactorFloor);

    // static std::unordered_set<unsigned long long> knownNonPrimes;
} 