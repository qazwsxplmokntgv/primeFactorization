#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <format>
#include <print>
#include <string>
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

    void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList) const;
    void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, 
        std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, 
        std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat) const;


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
