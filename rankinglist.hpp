#pragma once
#include <vector>
#include "factorization.hpp"

class RankingList {
public:
    RankingList(size_t maxSize_);

    //compares newItem against the existing ranked items, if any
    void checkAndRank(const factorizedNumInfo& newItem);

    const factorizedNumInfo& viewEntryAt(size_t idx) const;

protected:
    //should check newItem against the item ranked at idx
    //if true, newItem outranks the existing item
    virtual bool compareAgainst(const factorizedNumInfo& newItem, size_t idx) const = 0;
    
    std::vector<factorizedNumInfo> rankedItems;
    size_t maxSize;
};

class FastestRankingList : public RankingList {
public:
    FastestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const factorizedNumInfo& newItem, size_t idx) const;
};

class SlowestRankingList : public RankingList {
public:
    SlowestRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const factorizedNumInfo& newItem, size_t idx) const;
};

class MostTotalFactorsRankingList : public RankingList {
public:
    MostTotalFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const factorizedNumInfo& newItem, size_t idx) const;
};

class MostUniqueFactorsRankingList : public RankingList {
public:
    MostUniqueFactorsRankingList(size_t maxSize) : RankingList(maxSize) {}
private:
    bool compareAgainst(const factorizedNumInfo& newItem, size_t idx) const;
};
