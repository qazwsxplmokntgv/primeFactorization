#pragma once

#include <chrono>
#include <cmath>
#include <numeric>
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
    //maxInput == highest possible input
    StatSet(const unsigned long long inputCount, const unsigned long long maxInput);
    StatSet(const unsigned long long inputCount);
    void printout(void) const;
    void handleNewTime(factorizedNumInfo&& newFactorization);
    void completeFinalCalculations(void);

private:
    void initialize();


    const unsigned long long count, maxN;
    const size_t recordSize;

    //rank lists 
    FastestRankingList fastest;
    SlowestRankingList slowest;
    MostTotalFactorsRankingList mostFactors;
    MostUniqueFactorsRankingList mostUniqueFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> arithMean, harmonMean, geoMean, stdDev, iqMean;
    
    //counts of divisions of calcTimes
    TimeCategories categories;

    //timer from constructor through when complete final calculations is called
    std::chrono::duration<long double> fullSequenceRunDuration;
    std::chrono::steady_clock::time_point start;
    
    std::vector<std::chrono::duration<long double, std::milli>> timesData;
};

void printDivider(std::string&& leftHeader = "", std::string&& rightHeader = "");

//accepts a non integer index and calculates a weighted average of the adjacent elements
template<class T>
inline T getWeightedAverage(const double pos, const std::vector<T>& sortedList) {
    return sortedList[(size_t)(pos)] * (1 - (pos - floor(pos))) + sortedList[(size_t)(pos + (size_t)(sortedList.size() > 1))] * (pos - floor(pos));
}
