#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <vector>

class Factorization {
public:
    Factorization();
    void addNewFactor(const uint64_t base, const uint_fast8_t exp);
    
    //takes a prime factorization as returned by primeFactorization() and converts it to a string
    std::string asString() const; 
    
    const uint_fast8_t getFactorCount() const;
    const uint_fast8_t getUniqueFactorCount() const;

private:
    //base exponent pair
    struct factor {
        factor(uint64_t base_, uint_fast8_t exp_) : base(base_), exp(exp_) {}

        uint64_t base;
        //64 bit numbers cannot have factors with exp greater than floor(log2(2^64 - 1)) == 63
        //therefore this is guranteed to be sufficient theoretically through a 256 bit num
        uint_fast8_t exp; 
    };
    //see exp comment above
    //stores the total number of prime factors as sum(exp)
    uint_fast8_t factorCount = 0;

    std::vector<factor> factors;
};
