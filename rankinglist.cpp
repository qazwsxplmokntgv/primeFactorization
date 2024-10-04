#include "rankinglist.hpp"

RankingList::RankingList(size_t maxSize_) : maxSize(maxSize_) {
    rankedItems.resize(maxSize);
}

void RankingList::checkAndRank(const factorizedNumInfo& newItem) {
    size_t i = rankedItems.size() - 1;
    //if newItem does not beat the worst ranked item, return
    if (!compareAgainst(newItem, i)) return;
    //set i to the idx of the best item newItem beats
    while (i > 0 && compareAgainst(newItem, i - 1)) --i;
    //shift all items from the end through i inclusive back by 1
    for (size_t j = rankedItems.size() - 1; j > i; --j) rankedItems[j] = rankedItems[j - 1];
    //insert newItem into the opened spot
    rankedItems[i] = newItem;
}

const factorizedNumInfo& RankingList::viewEntryAt(size_t idx) const {
    return rankedItems[idx];
}


bool FastestRankingList::compareAgainst(const factorizedNumInfo& newItem, size_t idx) const {
    return newItem.calcTime < rankedItems[idx].calcTime || rankedItems[idx].calcTime == std::chrono::duration<long double, std::milli>(0);
}

bool SlowestRankingList::compareAgainst(const factorizedNumInfo& newItem, size_t idx) const {
    return newItem.calcTime > rankedItems[idx].calcTime;
}

bool MostTotalFactorsRankingList::compareAgainst(const factorizedNumInfo& newItem, size_t idx) const {
    return newItem.factorization.getFactorCount() > rankedItems[idx].factorization.getFactorCount()  
        //if tied, tie break with unique factor count
        || (newItem.factorization.getFactorCount() == rankedItems[idx].factorization.getFactorCount() 
        && newItem.factorization.getUniqueFactorCount() > rankedItems[idx].factorization.getUniqueFactorCount());
}

bool MostUniqueFactorsRankingList::compareAgainst(const factorizedNumInfo& newItem, size_t idx) const {
    return newItem.factorization.getUniqueFactorCount() > rankedItems[idx].factorization.getUniqueFactorCount()
        //if tied, tie break with total factor count
        || (newItem.factorization.getUniqueFactorCount() == rankedItems[idx].factorization.getUniqueFactorCount() 
        && newItem.factorization.getFactorCount() > rankedItems[idx].factorization.getFactorCount());;
}
