#pragma once

#include <chrono>
#include <cmath>
#include <iomanip>
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
    void noteNewTime(const factorizedNumInfo& newFactorization);
    void completeFinalCalculations(void);

private:
    void initialize(const size_t recordCount);

    //rank lists
    std::vector<factorizedNumInfo> fastest;
    std::vector<factorizedNumInfo> slowest;
    std::vector<factorizedNumInfo> mostFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> arithMean, harmonMean, geoMean, stdDev, iqMean;
    
    //counts of divisions of calcTimes
    timeCategories categories;

    std::chrono::steady_clock::time_point start;

    //timer from constructor through when complete final calculations is called
    std::chrono::duration<long double> fullSequenceRunDuration;
    
    //helper members
    std::vector<std::chrono::duration<long double, std::milli>> timesData;
    const unsigned long long count, maxN;
    std::chrono::duration<long double, std::milli>runningSum = std::chrono::duration<long, std::milli>(0);
};

