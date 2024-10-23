#include "rankinglist.hpp"

bool fastestComparator::operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const {
    return newItem.calcTime < existingItem.calcTime;
}

bool slowestComparator::operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const {
    return newItem.calcTime > existingItem.calcTime;
}

bool totalFactorsComparator::operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const {
    return newItem.factorization.getFactorCount() > existingItem.factorization.getFactorCount()  
        //if tied, tie break with unique factor count
        || (newItem.factorization.getFactorCount() == existingItem.factorization.getFactorCount() 
        && newItem.factorization.getUniqueFactorCount() > existingItem.factorization.getUniqueFactorCount());
}

bool uniqueFactorsComparator::operator()(const FactorCalculationInfo& newItem, const FactorCalculationInfo& existingItem) const {
    return newItem.factorization.getUniqueFactorCount() > existingItem.factorization.getUniqueFactorCount()
        //if tied, tie break with total factor count
        || (newItem.factorization.getUniqueFactorCount() == existingItem.factorization.getUniqueFactorCount() 
        && newItem.factorization.getFactorCount() > existingItem.factorization.getFactorCount());
}
