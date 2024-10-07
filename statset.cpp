#include "statset.hpp"

StatSet::StatSet(const uint64_t inputCount_) : inputCount(inputCount_), 
    fastest          (std::min(inputCount, (uint64_t)std::max((unsigned int)log10l(inputCount), 3u))), 
    slowest          (std::min(inputCount, (uint64_t)std::max((unsigned int)log10l(inputCount), 3u))), 
    mostFactors      (std::min(inputCount, (uint64_t)std::max((unsigned int)log10l(inputCount), 3u))), 
    mostUniqueFactors(std::min(inputCount, (uint64_t)std::max((unsigned int)log10l(inputCount), 3u))) {
    timesData.reserve(inputCount);
}


void StatSet::printout(void) const {
    //info blocks on the fastest and slowest calculation times for factorizations
    //shows calc time, the input, and the factorization itself
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted");
    RankingList::printRecordLists(fastest, slowest);
    
    //as above, for factorizations with the most factors
    printDivider("Factorizations With Most Total Factors", "Factorizations With Most Unique Factors");
    RankingList::printRecordLists(mostFactors, mostUniqueFactors, 
        [](size_t i, const RankingList& leftList){ return std::format("#{}: {} | {}", i + 1, leftList.viewEntryAt(i).factorization.getFactorCount(), leftList.viewEntryAt(i).calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {} | {}", i + 1, rightList.viewEntryAt(i).factorization.getUniqueFactorCount(), rightList.viewEntryAt(i).calcTime); });

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
    
    //prints an overview of the calculation time distribution
    printDivider("Counts (fastest applicable category only)");
    categories.printout();
}

void StatSet::handleNewTime(FactorCalculationInfo&& newFactorization) {
    //compares new item against each ranking list, inserting if and when appropriate
    fastest.checkAndRank(newFactorization);
    slowest.checkAndRank(newFactorization);
    mostFactors.checkAndRank(newFactorization);
    mostUniqueFactors.checkAndRank(newFactorization);
  
    //records the calculation time for calculation of a few statistical measures after completion
    timesData.emplace_back(newFactorization.calcTime);
}

void StatSet::completeFinalCalculations(void) {
    //avoid division by 0
    if (!inputCount) return;

    //places the collected times in order
    std::sort(timesData.begin(), timesData.end());

    //calculates the quartiles
    firstQuart = getWeightedAverage<std::chrono::duration<long double, std::milli>>((timesData.size() - 1) * .25, timesData);
    median =     getWeightedAverage<std::chrono::duration<long double, std::milli>>((timesData.size() - 1) * .5,  timesData);
    thirdQuart = getWeightedAverage<std::chrono::duration<long double, std::milli>>((timesData.size() - 1) * .75, timesData);

    //calculates the weight of the values at the ends of the iqr and adds them to the interquartile sum
    std::chrono::duration<long double, std::milli> interQuartileSum = 
        (.25 * (timesData.size() % 4)) *    //weight of the quartile edge elements ranges from 0-.75
        (timesData[timesData.size() / 4] +  //lower quartile edge element
        timesData[(timesData.size() - 1) - (timesData.size() / 4)]); //upper quartile edge element

    //calculates arithmetic mean ahead of the others in order to find variances
    arithMean = std::reduce(std::next(timesData.begin()), timesData.end(), timesData.front()) / inputCount;
    
    std::chrono::duration<long double, std::milli> sumReciprocals(0), sumLogs(0), sumSquaredDeviation(0);
    for (const std::chrono::duration<long double, std::milli> time : timesData) {
        //categorizes the time
        categories.increment(time.count()); 

        //totalling of several sums to later be used in calculating respective means
        sumReciprocals      += std::chrono::duration<long double, std::milli>(1 / time.count());
        sumLogs             += std::chrono::duration<long double, std::milli>(logl(time.count()));
        sumSquaredDeviation += std::chrono::duration<long double, std::milli>(powl(time.count() - arithMean.count(), 2));

        //only adds to the interquartile total if within the inner quartiles
        if (time.count() > firstQuart.count() && time.count() < thirdQuart.count()) interQuartileSum += time;
    }
    //calculates final values of means (besides arithmetic, already calculated in preparation for stdDev calculation), as well as stdDev
    harmonMean = std::chrono::duration<long double, std::milli>(1 / (sumReciprocals / inputCount).count());
    geoMean =    std::chrono::duration<long double, std::milli>(expl(sumLogs.count() / inputCount));
    iqMean =     std::chrono::duration<long double, std::milli>(interQuartileSum * 2. / inputCount);
    stdDev =     std::chrono::duration<long double, std::milli>(sqrtl(sumSquaredDeviation.count() / inputCount));
}

void printDivider(std::string&& leftHeader, std::string&& rightHeader) {
    //dictates the distance between left end of panel and the first char of the header
    static constexpr size_t indent = 3;
    //bold underline overline bright white series of dashes with header text inserted
    std::println("\033[1;4;53;97m{:-<{}}{:-<{}}{:-<{}}\033[0m", "", indent, leftHeader, panelWidth, rightHeader, panelWidth - indent);
}
