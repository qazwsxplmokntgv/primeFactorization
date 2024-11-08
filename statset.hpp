#pragma once

#include <algorithm>
#include <cstdint>
#include <chrono>
#include <cmath>
#include <numeric>
#include <string>
#include <format>
#include <print>
#include <map>
#include <vector>
#include "calculationinfo.hpp"
#include "rankinglist.hpp"
#include "timecategories.hpp"
#include "utils.hpp"

//collection of statistics tracked as primes factorizations are calculated
class StatSet {
public:
    StatSet(const size_t inputCount_);
    void printout(FILE* outStream = stdout) const;
    void handleNewFactorizationData(const FactorCalculationInfo& newFactorization);
    void completeFinalCalculations(void);

    const size_t getMaxValidInputCount(void) const;

    void addFactorsToCount(const Factorization& newFactorization);

private:
    const size_t inputCount;
    //value derived from input count dictating size of some records stored
    const size_t scale;
    //rank lists 
    RankingList<fastestComparator> fastest;
    RankingList<slowestComparator> slowest;
    RankingList<totalFactorsComparator> mostFactors;
    RankingList<uniqueFactorsComparator> mostUniqueFactors;

    //statistical facts
    std::chrono::duration<long double, std::milli> firstQuart, median, thirdQuart;
    std::chrono::duration<long double, std::milli> harmonMean, geoMean, iqMean, arithMean, stdDev;
    
    //counts of divisions of calcTimes
    TimeCategories timeCategories;

    //collection of all factors found, as if product of all inputs was factorized
    //Factorization class not used due to the need for frequent internal insertion and benefits of sorting gurantees
    //additionally because addition of exponents can result in exp exceeding 8 bits
    std::map<Factorization::base_t, unsigned int> allFactors;
    //flipped version that allFactors values will eventually be transferred to to filter out least common factors
    std::multimap<decltype(allFactors)::mapped_type, decltype(allFactors)::key_type, std::greater<>> mostCommonFactors;

    //vector of each individual calculation time
    std::vector<std::chrono::duration<long double, std::milli>> timesData;

};


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
