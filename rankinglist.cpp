#include "rankinglist.hpp"

RankingList::RankingList(size_t maxSize_) : maxSize(maxSize_) {
    rankedItems.resize(maxSize);
}

void RankingList::checkAndRank(const factorizedNumInfo& newItem) {
    //begin by referring to the index of the worst ranked item
    size_t i = rankedItems.size() - 1;
    //if newItem beats the worst ranked item,
    if (compareAgainst(newItem, i)) {
        //advance i down to the idx of the best item newItem beats
        while (i > 0 && compareAgainst(newItem, i - 1)) --i;
        //shift all items from the end through i inclusive back by 1
        for (size_t j = rankedItems.size() - 1; j > i; --j) rankedItems[j] = rankedItems[j - 1];
        //insert newItem into the opened spot
        rankedItems[i] = newItem;
    }
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

void RankingList::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList) {
    printRecordLists(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](size_t i, const RankingList& leftList){ return std::format("#{}: {}", i + 1, leftList.viewEntryAt(i).calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {}", i + 1, rightList.viewEntryAt(i).calcTime); });
}

void RankingList::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat) {
    for (size_t i = 0; i < std::min(leftRecordList.maxSize, rightRecordList.maxSize); ++i) {
        std::println("{:{}}{}\n{:{}}{}\n", 
            //info
            leftInfoFormat(i, leftRecordList), panelWidth,
            rightInfoFormat(i, rightRecordList),
            //factorizations themselves
            std::format("{} ={}", leftRecordList.viewEntryAt(i).n, leftRecordList.viewEntryAt(i).factorization.asString()), panelWidth,
            std::format("{} ={}", rightRecordList.viewEntryAt(i).n, rightRecordList.viewEntryAt(i).factorization.asString()));
    }
}