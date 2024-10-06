#pragma once

#include <print>
#include <functional>
#include <vector>
#include "factorcalculationinfo.hpp"
#include "utils.hpp"

class RankingList {
public:
    RankingList(size_t maxSize_);

    //compares newItem against the existing ranked items, if any
    void checkAndRank(const FactorCalculationInfo& newItem);

    const FactorCalculationInfo& viewEntryAt(size_t idx) const;

    static void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList);
    static void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, 
        std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, 
        std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat);

protected:
    //should check newItem against the item ranked at idx
    //if true, newItem outranks the existing item
    virtual bool compareAgainst(const FactorCalculationInfo& newItem, size_t idx) const = 0;
    
    std::vector<FactorCalculationInfo> rankedItems;
    size_t maxSize;
};

class FastestRankingList : public RankingList {
public:
    FastestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const FactorCalculationInfo& newItem, size_t idx) const;
};

class SlowestRankingList : public RankingList {
public:
    SlowestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const FactorCalculationInfo& newItem, size_t idx) const;
};

class MostTotalFactorsRankingList : public RankingList {
public:
    MostTotalFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const FactorCalculationInfo& newItem, size_t idx) const;
};

class MostUniqueFactorsRankingList : public RankingList {
public:
    MostUniqueFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const FactorCalculationInfo& newItem, size_t idx) const;
};
