#pragma once

#include <algorithm>
#include <print>
#include <functional>
#include <list>
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
    decltype(RankingList<Comp>::rankedItems)::const_iterator cbegin() const;
    decltype(RankingList<Comp>::rankedItems)::const_iterator cend() const;
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
decltype(RankingList<Comp>::rankedItems)::const_iterator RankingList<Comp>::cbegin() const {
    return rankedItems.cbegin();
}

template<class Comp>
decltype(RankingList<Comp>::rankedItems)::const_iterator RankingList<Comp>::cend() const {
    return rankedItems.cend();
}

template<class CompLeft, class CompRight>
inline void printRecordLists(const RankingList<CompLeft>& leftRecordList, const RankingList<CompRight>& rightRecordList) {
    printRecordLists<CompLeft, CompRight>(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](size_t i, const RankingList<CompLeft>& leftList){ return std::format("#{}: {}", i + 1, std::next(leftList.cbegin(), i)->calcTime); },
        [](size_t i, const RankingList<CompRight>& rightList){ return std::format("#{}: {}", i + 1, std::next(rightList.cbegin(), i)->calcTime); });
}

template<class CompLeft, class CompRight>
inline void printRecordLists(const RankingList<CompLeft>& leftRecordList, const RankingList<CompRight>& rightRecordList, std::function<const std::string(size_t index, const RankingList<CompLeft>& list)>&& leftInfoFormat, std::function<const std::string(size_t index, const RankingList<CompRight>& list)>&& rightInfoFormat) {
    auto leftIt = leftRecordList.cbegin(), rightIt = rightRecordList.cbegin();
    for (unsigned i = 0; leftIt != leftRecordList.cend() && rightIt != rightRecordList.cend(); ++i) {
        std::println("{:{}}{}\n{:{}}{}\n", 
            //first line of info as specified in parameters
            leftInfoFormat(i, leftRecordList), panelWidth,
            rightInfoFormat(i, rightRecordList),
            //second line is always n == factorization of n, doesn't need customization info 
            std::format("{} ={}", leftIt->n, leftIt->factorization.asString()), panelWidth,
            std::format("{} ={}", rightIt->n, rightIt->factorization.asString())
        );
        
        std::advance(leftIt, 1);
        std::advance(rightIt, 1);
    }
}
