#pragma once

#include <print>
#include <functional>
#include <list>
#include <vector>
#include "calculationinfo.hpp"
#include "utils.hpp"

class RankingList {
public:
    RankingList(size_t maxSize_);

    //compares newItem against the existing ranked items, if any
    void rankIfApplicable(const FactorCalculationInfo& newItem);

    std::list<FactorCalculationInfo>::const_iterator cbegin() const;
    std::list<FactorCalculationInfo>::const_iterator cend() const;

    static void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList);
    static void printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, 
        std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, 
        std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat);

protected:
    //should check newItem against the item ranked at pos
    //if true, newItem outranks the existing item
    virtual bool outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const = 0;

    bool isFilled() const;
    
    std::list<FactorCalculationInfo> rankedItems;
    size_t maxSize;
};

class FastestRankingList : public RankingList {
public:
    FastestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const;
};

class SlowestRankingList : public RankingList {
public:
    SlowestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const;
};

class MostTotalFactorsRankingList : public RankingList {
public:
    MostTotalFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const;
};

class MostUniqueFactorsRankingList : public RankingList {
public:
    MostUniqueFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const;
};
