#pragma once

#include <print>
#include <functional>
#include <set>
#include "calculationinfo.hpp"
#include "utils.hpp"

struct fastestComparator       { bool operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const; };
struct slowestComparator       { bool operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const; };
struct totalFactorsComparator  { bool operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const; };
struct uniqueFactorsComparator { bool operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const; };

template<class Comp>
class RankingList {
public:
    RankingList(size_t maxSize_) : maxSize(maxSize_), rankedItems(Comp()) {};

    //compares newItem against the existing ranked items, and inserts in order
    void rankIfApplicable(const FactorCalculationInfo& newItem);

private:
    bool isFilled(void) const;

    bool outranksWorstItem(const FactorCalculationInfo& newItem) const;

    size_t maxSize;
    std::multiset<FactorCalculationInfo, Comp> rankedItems;

public:
    using container_t = decltype(RankingList<Comp>::rankedItems);
    container_t::const_iterator cbegin() const;
    container_t::const_iterator cend() const;
};

template<class Comp>
void RankingList<Comp>::rankIfApplicable(const FactorCalculationInfo& newItem) {
    if (!isFilled()) rankedItems.insert(newItem);
    else if (outranksWorstItem(newItem)) {
        rankedItems.erase(std::prev(rankedItems.end()));
        rankedItems.insert(newItem);
    }
}

template<class Comp>
bool RankingList<Comp>::isFilled() const {
    return rankedItems.size() == maxSize;
}

template<class Comp>
bool RankingList<Comp>::outranksWorstItem(const FactorCalculationInfo& newItem) const {
    return rankedItems.key_comp()(newItem, *std::prev(rankedItems.cend()));
}

template<class Comp>
RankingList<Comp>::container_t::const_iterator RankingList<Comp>::cbegin() const {
    return rankedItems.cbegin();
}

template<class Comp>
RankingList<Comp>::container_t::const_iterator RankingList<Comp>::cend() const {
    return rankedItems.cend();
}

template<class CompLeft, class CompRight>
inline void printRecordLists(const RankingList<CompLeft>& leftRecordList, const RankingList<CompRight>& rightRecordList, FILE* outStream = stdout) {
    printRecordLists<CompLeft, CompRight>(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](const RankingList<CompLeft>::container_t::const_iterator& leftIt){ return std::format("{}", leftIt->calcTime); },
        [](const RankingList<CompRight>::container_t::const_iterator& rightIt){ return std::format("{}", rightIt->calcTime); }, outStream
    );
}

template<class CompLeft, class CompRight>
inline void printRecordLists(const RankingList<CompLeft>& leftRecordList, const RankingList<CompRight>& rightRecordList, 
    std::function<const std::string(const typename RankingList<CompLeft>::container_t::const_iterator& leftIt)>&& leftInfoFormat, 
    std::function<const std::string(const typename RankingList<CompRight>::container_t::const_iterator& rightIt)>&& rightInfoFormat, 
    FILE* outStream = stdout) {
    
    unsigned rank = 1;
    for (auto leftIt = leftRecordList.cbegin(), rightIt = rightRecordList.cbegin(); leftIt != leftRecordList.cend() && rightIt != rightRecordList.cend(); ++rank, std::advance(leftIt, 1), std::advance(rightIt, 1)) {
        std::println(outStream, "{:{}}{}\n{:{}}{}\n", 
            //first line of info as specified in parameters
            std::format("#{}: {}", rank, leftInfoFormat(leftIt)), panelWidth,
            std::format("#{}: {}", rank, rightInfoFormat(rightIt)),
            //second line is always n == factorization of n, doesn't need custom formatting function 
            std::format("{} ={}", leftIt->n, leftIt->factorization.asString()), panelWidth,
            std::format("{} ={}", rightIt->n, rightIt->factorization.asString())
        );
    }
}
