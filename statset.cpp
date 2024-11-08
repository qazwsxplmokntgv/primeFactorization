#include "statset.hpp"

StatSet::StatSet(const size_t inputCount_) :
    inputCount(inputCount_), 
    //scale should never be less than 3 unless there are fewer than 3 inputs, and should scale as inputCount grows (specifically in accordance to log10 works well and is pretty intuitive for users)
    scale(std::min(inputCount, static_cast<size_t>(std::max(log10(inputCount), 3.)))), 
    fastest(scale), 
    slowest(scale),
    mostFactors(scale), 
    mostUniqueFactors(scale) {
    timesData.reserve(inputCount);
}


void StatSet::printout(FILE* outStream) const {
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted", outStream);
    printRecordLists<fastestComparator, slowestComparator>(fastest, slowest, outStream);
    
    printDivider("Factorizations With Most Total Factors", "Factorizations With Most Unique Factors", outStream);
    printRecordLists<totalFactorsComparator, uniqueFactorsComparator>(mostFactors, mostUniqueFactors, 
        [](const RankingList<totalFactorsComparator>::container_t::const_iterator& leftIt ){ return std::format("{} | {}", leftIt->factorization.getFactorCount(), leftIt->calcTime); },
        [](const RankingList<uniqueFactorsComparator>::container_t::const_iterator& rightIt){ return std::format("{} | {}", rightIt->factorization.getUniqueFactorCount(), rightIt->calcTime); }, outStream);

    printDivider("Calculation Times", outStream);
    std::println(outStream, "{:{}}{}", 
        std::format("{}{}", "Q0: ", inputCount ? timesData.front() : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
        std::format("{}{}", "Harmonic Mean:      ", harmonMean));
    std::println(outStream, "{:{}}{}", 
        std::format("{}{}", "Q1: ", firstQuart), miniPanelWidth, 
        std::format("{}{}", "Geometric Mean:     ", geoMean));
    std::println(outStream, "{:{}}{}", 
        std::format("{}{}", "Q2: ", median), miniPanelWidth, 
        std::format("{}{}", "Interquartile Mean: ", iqMean));
    std::println(outStream, "{:{}}{}", 
        std::format("{}{}", "Q3: ", thirdQuart), miniPanelWidth, 
        std::format("{}{}", "Arithmetic Mean:    ", arithMean));
    std::println(outStream, "{:{}}{}", 
        std::format("{}{}", "Q4: ", inputCount ? timesData.back() : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
        std::format("{}{}", "Standard Deviation: ", stdDev));
    
    printDivider("Counts (fastest applicable category only)", outStream);
    timeCategories.printout(outStream);

    printDivider("Most Common Prime Factors", outStream);
    int unreadyForNewline = 0;
    for (const auto& [count, base] : mostCommonFactors) {
        std::print(outStream, "{:{}}", std::format("{}: {}", base, count), miniPanelWidth / 3);
        if (!(++unreadyForNewline %= 12)) std::println(outStream);
    }
    std::println(outStream);
}

void StatSet::handleNewFactorizationData(const FactorCalculationInfo& newFactorization) {
    fastest.rankIfApplicable(newFactorization);
    slowest.rankIfApplicable(newFactorization);
    mostFactors.rankIfApplicable(newFactorization);
    mostUniqueFactors.rankIfApplicable(newFactorization);
  
    addFactorsToCount(newFactorization.factorization);

    timesData.push_back(newFactorization.calcTime);
}

void StatSet::completeFinalCalculations(void) {
    //avoid division by 0 - leaves duration values at default constructed 0
    if (!inputCount) return;

    std::ranges::sort(timesData, std::less());

    firstQuart = interpolateDataAtPercentile<std::chrono::duration<long double, std::milli>>(timesData, .25);
    median =     interpolateDataAtPercentile<std::chrono::duration<long double, std::milli>>(timesData, .5);
    thirdQuart = interpolateDataAtPercentile<std::chrono::duration<long double, std::milli>>(timesData, .75);

    //because the edge elements of the inner quartiles may have unique weights compared to the other elements, they are handled separately
    std::chrono::duration<long double, std::milli> interQuartileSum = 
        (.25 * (timesData.size() % 4)) *    //<-- quartile edge elements weight follows this expr
        (timesData[timesData.size() / 4] +  //lower quartile edge element
        timesData[timesData.size() - 1 - (timesData.size() / 4)]); //upper quartile edge element

    //calculated ahead due to use in stdDev calculation 
    arithMean = std::reduce(std::next(timesData.begin()), timesData.end(), timesData.front()) / inputCount;
    
    std::chrono::duration<long double, std::milli> sumReciprocals(0), sumLogs(0), sumSquaredDeviations(0);
    for (const std::chrono::duration<long double, std::milli> time : timesData) {
        timeCategories.incrementAppropriateCategory(time.count()); 

        sumReciprocals       += std::chrono::duration<long double, std::milli>(1. / time.count());
        sumLogs              += std::chrono::duration<long double, std::milli>(logl(time.count()));
        sumSquaredDeviations += std::chrono::duration<long double, std::milli>(powl(time.count() - arithMean.count(), 2));

        if (time.count() > firstQuart.count() && time.count() < thirdQuart.count()) interQuartileSum += time;
    }
    harmonMean = std::chrono::duration<long double, std::milli>(1. / (sumReciprocals / inputCount).count());
    geoMean =    std::chrono::duration<long double, std::milli>(expl(sumLogs.count() / inputCount));
    iqMean =     std::chrono::duration<long double, std::milli>(interQuartileSum * 2. / inputCount);
    stdDev =     std::chrono::duration<long double, std::milli>(sqrtl(sumSquaredDeviations.count() / inputCount));

    for (const auto& [base, exp] : allFactors) {
        mostCommonFactors.emplace(exp, base);
        if (mostCommonFactors.size() > scale * 12) 
            mostCommonFactors.erase(std::prev(mostCommonFactors.end()));
    }
}

const size_t StatSet::getMaxValidInputCount(void) const {
    return timesData.max_size();
}

void StatSet::addFactorsToCount(const Factorization& newFactorization) {
    for (const auto& newFactor : newFactorization.viewFactors()) 
        allFactors[newFactor.base] += newFactor.exp;

    
}
