#include "rankinglist.hpp"

RankingList::RankingList(size_t maxSize_) : maxSize(maxSize_) {}

void RankingList::rankIfApplicable(const FactorCalculationInfo& newItem) {
    auto rankedItem { rankedItems.cend() };
    //to prevent checks against empty lists
    if (!rankedItems.empty()) {
        //if newItem ranks below the worst already ranked item, newItem can either be simply pushed back or discarded depending on room
        if (!outranksNthItem(newItem, std::prev(rankedItem))) {
            if (!isFilled()) 
                rankedItems.push_back(newItem);
            return;
        }
        //prevents wasteful double checking against the worst ranked item
        else std::advance(rankedItem, -1);
    }

    //advances rankedItem to the highest ranked item outranked by newItem
    for (; rankedItem != rankedItems.cbegin() && outranksNthItem(newItem, std::prev(rankedItem)); std::advance(rankedItem, -1));

    rankedItems.insert(rankedItem, newItem);

    //if an entry falls out of the rankings, it can be discarded
    if (rankedItems.size() > maxSize) rankedItems.pop_back();
}

std::list<FactorCalculationInfo>::const_iterator RankingList::cbegin() const {
    return rankedItems.cbegin();
}

std::list<FactorCalculationInfo>::const_iterator RankingList::cend() const {
    return rankedItems.cend();
}

bool FastestRankingList::outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const {
    return newItem.calcTime < pos->calcTime;
}

bool SlowestRankingList::outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const {
    return newItem.calcTime > pos->calcTime;
}

bool MostTotalFactorsRankingList::outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const {
    return newItem.factorization.getFactorCount() > pos->factorization.getFactorCount()  
        //if tied, tie break with unique factor count
        || (newItem.factorization.getFactorCount() == pos->factorization.getFactorCount() 
        && newItem.factorization.getUniqueFactorCount() > pos->factorization.getUniqueFactorCount());
}

bool MostUniqueFactorsRankingList::outranksNthItem(const FactorCalculationInfo& newItem, std::list<FactorCalculationInfo>::const_iterator pos) const {
    return newItem.factorization.getUniqueFactorCount() > pos->factorization.getUniqueFactorCount()
        //if tied, tie break with total factor count
        || (newItem.factorization.getUniqueFactorCount() == pos->factorization.getUniqueFactorCount() 
        && newItem.factorization.getFactorCount() > pos->factorization.getFactorCount());
}

void RankingList::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList) {
    printRecordLists(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](size_t i, const RankingList& leftList){ return std::format("#{}: {}", i + 1, std::next(leftList.cbegin(), i)->calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {}", i + 1, std::next(rightList.cbegin(), i)->calcTime); });
}

void RankingList::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat) {
    std::list<FactorCalculationInfo>::const_iterator leftIt = leftRecordList.cbegin(), rightIt = rightRecordList.cbegin();
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

bool RankingList::isFilled() const {
    return rankedItems.size() == maxSize;
}
