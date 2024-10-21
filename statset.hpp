#pragma once

#include <algorithm>
#include <cstdint>
#include <chrono>
#include <cmath>
#include <numeric>
#include <string>
#include <format>
#include <print>
#include <vector>
#include "rankinglist.hpp"
#include "timecategories.hpp"
#include "utils.hpp"

//collection of statistics tracked as primes factorizations are calculated
class StatSet {
public:
    //inputCount == number of nums whose factorizations will be considered
    StatSet(const size_t inputCount);
    void printout(void) const;
    void handleNewTime(FactorCalculationInfo&& newFactorization);
    void completeFinalCalculations(void);

    const size_t getMaxValidInputCount(void) const;

private:
    const size_t inputCount;
    //rank lists 
    FastestRankingList fastest;
    SlowestRankingList slowest;
    MostTotalFactorsRankingList mostFactors;
    MostUniqueFactorsRankingList mostUniqueFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> harmonMean, geoMean, iqMean, arithMean, stdDev;
    
    //counts of divisions of calcTimes
    TimeCategories timeCategories;

    //vector of each individual calculation time
    std::vector<std::chrono::duration<long double, std::milli>> timesData;
};

void printDivider(std::string&& leftHeader = "", std::string&& rightHeader = "");

//takes a target fractional percentile (0-1) and calculates a weighted average from the closest elements to that target
template<class T>
inline T interpolateDataAtPercentile(const std::vector<T>& sortedList, const double fractionalPercentile) {
    //prevents attempts to access 2 distinctly indexed elements nearest to a target when 2 such elements do not exist 
    if (sortedList.size() < 2) 
        return sortedList.empty() ? T(0) : sortedList.front();
    else {
        const double pos = (sortedList.size() - 1) * fractionalPercentile;
        const auto pFirst = sortedList.cbegin() + static_cast<size_t>(pos);
        return (*pFirst) + ((pos - floor(pos)) * ((*std::next(pFirst)) - (*pFirst)));
    } 
}
