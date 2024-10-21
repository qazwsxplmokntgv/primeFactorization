#include "statset.hpp"

StatSet::StatSet(const size_t inputCount_) : inputCount(inputCount_), 
    //size of ranking lists should never be less than 3 unless there are fewer than 3 inputs, and should scale as inputCount grows (specifically in accordance to log10 works well)
    fastest          (std::min(inputCount, static_cast<size_t>(std::max(log10(inputCount), 3.)))), 
    slowest          (std::min(inputCount, static_cast<size_t>(std::max(log10(inputCount), 3.)))), 
    mostFactors      (std::min(inputCount, static_cast<size_t>(std::max(log10(inputCount), 3.)))), 
    mostUniqueFactors(std::min(inputCount, static_cast<size_t>(std::max(log10(inputCount), 3.)))) {
    timesData.reserve(inputCount);
}


void StatSet::printout(void) const {
    //info blocks on the fastest and slowest calculation times for factorizations
    //shows calc time, the input, and the factorization itself
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted");
    RankingList::printRecordLists(fastest, slowest);
    
    //same as above, for factorizations with the most factors
    printDivider("Factorizations With Most Total Factors", "Factorizations With Most Unique Factors");
    RankingList::printRecordLists(mostFactors, mostUniqueFactors, 
        [](size_t i, const RankingList& leftList ){ return std::format("#{}: {} | {}", i + 1, std::next(leftList.cbegin(),  i)->factorization.getFactorCount(), std::next(leftList.cbegin(), i)->calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {} | {}", i + 1, std::next(rightList.cbegin(), i)->factorization.getUniqueFactorCount(), std::next(rightList.cbegin(), i)->calcTime); });

    //various statistical facts regarding calculation times 
    printDivider("Calculation Times");
    std::println("{:{}}{}", 
        std::format("{}{}", "Q0: ", inputCount ? timesData.front() : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
        std::format("{}{}", "Harmonic Mean:      ", harmonMean));
    std::println("{:{}}{}", 
        std::format("{}{}", "Q1: ", firstQuart), miniPanelWidth, 
        std::format("{}{}", "Geometric Mean:     ", geoMean));
    std::println("{:{}}{}", 
        std::format("{}{}", "Q2: ", median), miniPanelWidth, 
        std::format("{}{}", "Interquartile Mean: ", iqMean));
    std::println("{:{}}{}", 
        std::format("{}{}", "Q3: ", thirdQuart), miniPanelWidth, 
        std::format("{}{}", "Arithmetic Mean:    ", arithMean));
    std::println("{:{}}{}", 
        std::format("{}{}", "Q4: ", inputCount ? timesData.back() : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
        std::format("{}{}", "Standard Deviation: ", stdDev));
    
    //overview of the calculation time distribution
    printDivider("Counts (fastest applicable category only)");
    timeCategories.printout();
}

void StatSet::handleNewTime(FactorCalculationInfo&& newFactorization) {
    fastest.rankIfApplicable(newFactorization);
    slowest.rankIfApplicable(newFactorization);
    mostFactors.rankIfApplicable(newFactorization);
    mostUniqueFactors.rankIfApplicable(newFactorization);
  
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
}

const size_t StatSet::getMaxValidInputCount(void) const {
    return timesData.max_size();
}

void printDivider(std::string&& leftHeader, std::string&& rightHeader) {
    static constexpr size_t indent = 3;
    //bold, underline, overline, bright-white series of fill dashes with header text inserted left aligned to each panel, 
    //indented according to indent
    std::println("\033[1;4;53;97m{:-<{}}{:-<{}}{:-<{}}\033[0m", "", indent, leftHeader, panelWidth, rightHeader, panelWidth - indent);
}
