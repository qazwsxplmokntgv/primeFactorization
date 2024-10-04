#pragma once

#include <chrono>
#include <cmath>
#include <functional>
#include <format>
#include <print>
#include <string>
#include <vector>
#include "utils.hpp"

//collection of statistics tracked as primes factorizations are calculated
class StatSet {
public:
    //inputCount == number of nums whose factorizations will be considered
    //recordCount == max number of fastest/slowest records to store
    //maxInput == highest possible input
    StatSet(const unsigned long long inputCount, const size_t recordCount, const unsigned long long maxInput);
    StatSet(const unsigned long long inputCount, const size_t recordCount);
    void printout(void) const;
    void handleNewTime(factorizedNumInfo&& newFactorization);
    void completeFinalCalculations(void);

private:
    void initialize(const size_t recordCount);

    void printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList) const;
    void printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList, 
        std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)>&& leftInfoFormat, 
        std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)>&& rightInfoFormat) const;

    //rank lists
    std::vector<factorizedNumInfo> fastest;
    std::vector<factorizedNumInfo> slowest;
    std::vector<factorizedNumInfo> mostFactors;
    std::vector<factorizedNumInfo> mostUniqueFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> arithMean, harmonMean, geoMean, stdDev, iqMean;
    
    //counts of divisions of calcTimes
    TimeCategories categories;

    //timer from constructor through when complete final calculations is called
    std::chrono::duration<long double> fullSequenceRunDuration;
    std::chrono::steady_clock::time_point start;
    
    const unsigned long long count;
    const size_t recordSize;
    const unsigned long long maxN;

    std::vector<std::chrono::duration<long double, std::milli>> timesData;
    std::chrono::duration<long double, std::milli>runningSum = std::chrono::duration<long, std::milli>(0);
};

//compares newItem against existingRankings, inserting if applicable
//comparison should return true for objects that should be higher ranked
void rankIfApplicable(const factorizedNumInfo& newItem, std::vector<factorizedNumInfo>& existingRankings, std::function<bool(const factorizedNumInfo&, const factorizedNumInfo&)>&& comparison);