#pragma once

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
    StatSet(const uint64_t inputCount);
    void printout(void) const;
    void handleNewTime(FactorCalculationInfo&& newFactorization);
    void completeFinalCalculations(void);

private:
    const uint64_t inputCount;
    //rank lists 
    FastestRankingList fastest;
    SlowestRankingList slowest;
    MostTotalFactorsRankingList mostFactors;
    MostUniqueFactorsRankingList mostUniqueFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> harmonMean, geoMean, iqMean, arithMean, stdDev;
    
    //counts of divisions of calcTimes
    TimeCategories categories;

    //vector of each individual calculation time
    std::vector<std::chrono::duration<long double, std::milli>> timesData;
};

void printDivider(std::string&& leftHeader = "", std::string&& rightHeader = "");

//accepts a non integer index and calculates a weighted average of the adjacent elements
template<class T>
inline T getWeightedAverage(const double pos, const std::vector<T>& sortedList) {
    return sortedList[(size_t)(pos)] * (1 - (pos - floor(pos))) + sortedList[(size_t)(pos + (size_t)(sortedList.size() > 1))] * (pos - floor(pos));
}
