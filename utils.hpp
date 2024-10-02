#pragma once

#include <array>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <format>
#include <string>
#include <format>
#include <string>
#include <iostream>
#include <vector>

constexpr size_t panelWidth = 64;
constexpr size_t miniPanelWidth = 26;
constexpr size_t miniPanelWidth = 26;

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
    void printoutout(void) const;
    
private:
    struct subdivision
    {
        std::string displayText;
        long double floorMilliEquivalent;
        int count;
    };
    
    constexpr static int subdivisionCount = 16;

    std::array<subdivision, subdivisionCount> subdivisions { subdivision
        { "<   1   us: ", 0       , 0 },
        { ">=  1   us: ", .0001   , 0 },
        { ">= .25  ms: ", .25     , 0 },
        { ">= .5   ms: ", .5      , 0 },
        { ">=  1   ms: ", 1       , 0 },
        { ">= .25 sec: ", 250     , 0 },
        { ">= .5  sec: ", 500     , 0 },
        { ">=  1  sec: ", 1000    , 0 },

        { ">=  3 sec: ",  3000    , 0 },
        { ">=  5 sec: ",  5000    , 0 },
        { ">= 10 sec: ",  10000   , 0 },
        { ">= 30 sec: ",  30000   , 0 },
        { ">=  1 min: ",  60000   , 0 },
        { ">=  5 min: ",  3000000 , 0 },
        { ">= 10 min: ",  6000000 , 0 },
        { ">= 30 min: ",  18000000, 0 } 
    };
};

void printDivider(const std::string&& leftHeader = "", const std::string&& rightHeader = "");
 
//takes a prime factorization as returned by primeFactorization() and converts it to a string
std::string toString(const std::vector<factor>& factorization); 

//compares newItem against existingRankings, inserting if applicable
//comparison should return true for objects that should be higher ranked
void rankIfApplicable(const factorizedNumInfo& newItem, std::vector<factorizedNumInfo>& existingRankings, const std::function<bool(const factorizedNumInfo&, const factorizedNumInfo&)>&& comparison);

unsigned int getFactorCount(const std::vector<factor>& factorization);
