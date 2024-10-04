#pragma once

#include <cstdint>
#include <chrono>
#include <format>
#include <string>
#include <vector>

class Factorization {
public:
    Factorization();
    void addNewFactor(const unsigned long long base, const uint_fast8_t exp);
    
    //takes a prime factorization as returned by primeFactorization() and converts it to a string
    std::string asString() const; 
    
    const int getFactorCount() const;
    const int getUniqueFactorCount() const;

private:
    //the product of the first 17 primes exceeds the 64 bit unsigned long long 
    //therefore the factorization of a 64 bit unsigned integer cannot have more than 16 distinct factors
    constexpr static int maxDistinctFactors = 16;
    //base exponent pair
    struct factor {
        factor(unsigned long long base_, uint_fast8_t exp_) : base(base_), exp(exp_) {}

        unsigned long long base;
        //64 bit numbers cannot have factors with exp greater than floor(log2(2^64 - 1)) == 63
        //therefore this is guranteed to be sufficient theoretically through a 256 bit num
        uint_fast8_t exp; 
    };
    //see exp comment above
    //stores the total number of prime factors as sum(exp)
    uint_fast8_t factorCount = 0;

    std::vector<factor> factors;

    //std::array<factor, maxDistinctFactors> factors;
    //size_t factorsArrayBack = 0;
};

//used to store information on noteworthy factorizations for use in concluding statistical printouts
struct factorizedNumInfo {
    unsigned long long n;
    Factorization factorization;
    std::chrono::duration<long double, std::milli> calcTime;
};