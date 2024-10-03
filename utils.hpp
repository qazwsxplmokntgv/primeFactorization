#pragma once

#include <array>
#include <format>
#include <print>
#include <string>
#include "factorization.hpp"

constexpr size_t panelWidth = 64;
constexpr size_t miniPanelWidth = panelWidth / 2;

class TimeCategories {
public:
    void increment(const long double timeMs);
    //output contents of the object to stdout
    void printout(void) const;
    
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

