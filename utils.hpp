#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

constexpr size_t panelWidth = 64;

//base exponent pair
struct factor {
    factor(unsigned long long base_, uint_fast8_t exp_) : base(base_), exp(exp_) {}
    unsigned long long base;
    uint_fast8_t exp;
};

//used to store information on noteworthy factorizations for use in concluding statistical printouts
struct factorizedNumInfo {
    unsigned long long n;
    std::vector<factor> factorization;
    std::chrono::duration<long double, std::milli> calcTime;
};

class timeCategories {
public:
    void increment(const long double timeMs);
    //output contents of the object to cout
    void print(void) const;
    
private:
    int subMicro = 0, microOrMore = 0, 
        quarterMilliOrMore = 0, halfMilliOrMore = 0, milliOrMore = 0, 
        quarterSecOrMore = 0, halfSecOrMore = 0, secOrMore = 0, 
        threeSecOrMore = 0, fiveSecOrMore = 0, tenSecOrMore = 0, thirtySecOrMore = 0, 
        minOrMore = 0, fiveMinOrMore = 0, tenMinOrMore = 0, thirtyMinOrMore = 0;
};

void printDivider(const std::string&& leftHeader = "", const std::string&& rightHeader = "");
 
//takes a prime factorization as returned by primeFactorization() and prints it to stream
void printFactorization(const std::vector<factor>& factorization, std::ostream& stream = std::cout); 

//compares newItem against existingRankings, inserting if applicable
//comparison should return true for objects that should be higher ranked
void rankIfApplicable(const factorizedNumInfo& newItem, std::vector<factorizedNumInfo>& existingRankings, const std::function<bool(const factorizedNumInfo&, const factorizedNumInfo&)>&& comparison);

unsigned int getFactorCount(const std::vector<factor>& factorization);
