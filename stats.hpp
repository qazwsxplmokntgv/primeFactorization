#pragma once

#include <chrono>
#include <cmath>
#include <format>
#include <iostream>
#include <vector>
#include "utils.hpp"

//collection of statistics tracked as primes factorizations are calculated
class statCollection {
public:
    //inputCount == number of nums whose factorizations will be considered
    //recordCount == max number of fastest/slowest records to store
    //maxInput == highest possible input
    statCollection(const unsigned long long inputCount, const size_t recordCount, const unsigned long long maxInput);
    statCollection(const unsigned long long inputCount, const size_t recordCount);
    void printout(void) const;
    void handleNewTime(factorizedNumInfo&& newFactorization);
    void completeFinalCalculations(void);

private:
    void initialize(const size_t recordCount);

    void printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList) const;
    void printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList, 
        std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)> leftInfoFormat, 
        std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)> rightInfoFormat) const;

    //rank lists
    std::vector<factorizedNumInfo> fastest;
    std::vector<factorizedNumInfo> slowest;
    std::vector<factorizedNumInfo> mostFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> arithMean, harmonMean, geoMean, stdDev, iqMean;
    
    //counts of divisions of calcTimes
    timeCategories categories;

    //timer from constructor through when complete final calculations is called
    std::chrono::duration<long double> fullSequenceRunDuration;
    std::chrono::steady_clock::time_point start;
    
    const unsigned long long count;
    const size_t recordSize;
    const unsigned long long maxN;

    std::vector<std::chrono::duration<long double, std::milli>> timesData;
    std::chrono::duration<long double, std::milli>runningSum = std::chrono::duration<long, std::milli>(0);
};

