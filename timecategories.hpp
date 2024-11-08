#pragma once

#include <array>
#include <format>
#include <print>
#include <string>
#include "utils.hpp"

class TimeCategories {
public:
    void incrementAppropriateCategory(const long double timeMs);
    //output contents of the object to stdout
    void printout(FILE* outStream = stdout) const;
    
private:
    struct subdivision
    {
        std::string displayText;
        long double milliEquiv;
        int count;
    };
    
    constexpr static int
        columnCount = 4, //number of columns to display in
        subdivisionCount = 20; //must be divisible by columnCount


    std::array<subdivision, subdivisionCount> subdivisions { subdivision
        { "<  1  μs: ", .001, 0 },
        { "< 10  μs: ", .01 , 0 },
        { "< ⅟8  ms: ", .125 , 0 },
        { "< ⅟4  ms: ", .25  , 0 },
        { "< ⅟2  ms: ", .5   , 0 },
        { "<  1  ms: ",     1, 0 },
        { "< 10  ms: ",    10, 0 },
        { "< ⅟4 sec: ",   250, 0 },
        { "< ⅟2 sec: ",   500, 0 },
        { "<  1 sec: ",  1000, 0 },
        
        //uses the value of the next category (< that == >= period shown in strings for purposes of this class)
        { ">=  1 sec: ",     3000, 0 },
        { ">=  3 sec: ",     5000, 0 },
        { ">=  5 sec: ",    10000, 0 },
        { ">= 10 sec: ",    30000, 0 },
        { ">= 30 sec: ",    60000, 0 },
        { ">=  1 min: ",  3000000, 0 },
        { ">=  5 min: ",  6000000, 0 },
        { ">= 10 min: ", 18000000, 0 },
        { ">= 30 min: ", 36000000, 0 },
        { ">=  1  hr: ", std::numeric_limits<long double>::infinity(), 0 } 
    };
};
