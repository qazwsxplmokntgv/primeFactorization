#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <vector>
#include <span>

class Factorization {
public:
    using base_t = uint64_t;
    using exp_t = uint_fast8_t;

private:    
    //base exponent pair
    struct factor {
        factor(base_t base_, exp_t exp_) : base(base_), exp(exp_) {}

        base_t base;
        //64 bit numbers cannot have factors with exp greater than floor(log2(2^64 - 1)) == 63
        //therefore this is guranteed to be sufficient theoretically through a 256 bit num
        exp_t exp; 
    };

public:
    Factorization();
    using container_t = std::vector<factor>;
    
    void addNewFactor(const base_t base, const exp_t exp);
    
    //takes a prime factorization as returned by primeFactorization() and converts it to a string
    std::string asString(void) const; 
    
    const uint_fast8_t getFactorCount(void) const;
    const uint_fast8_t getUniqueFactorCount(void) const;

    const container_t& viewFactors(void) const;

private:
    //stores the total number of prime factors as sum(exp)
    uint_fast8_t factorCount = 0;

    container_t factors;
};
