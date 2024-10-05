#include "statset.hpp"

StatSet::StatSet(const unsigned long long inputCount, const unsigned long long maxInput) : 
    count(inputCount), maxN(maxInput), recordSize(std::max((unsigned int)log10l(count), 3u)),
    fastest(recordSize), slowest(recordSize), 
    mostFactors(recordSize), mostUniqueFactors(recordSize) {
    initialize();
}

StatSet::StatSet(const unsigned long long inputCount) : 
    count(inputCount), maxN(0ull), recordSize(std::max((unsigned int)log10l(count), 3u)),
    fastest(recordSize), slowest(recordSize), 
    mostFactors(recordSize), mostUniqueFactors(recordSize) {
    initialize();
}

void StatSet::printout(void) const {
    //stat printout header
    printDivider();
    std::println("{} factorizations{} calculated in {}.\n", count, maxN ? std::format(" of numbers <= {}", maxN) : "", fullSequenceRunDuration);

    //info blocks on the fastest and slowest calculation times for factorizations
    //shows calc time, the input, and the factorization itself
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted");
    printRecordLists(fastest, slowest);

    //as above, for factorizations with the most factors
    printDivider("Factorizations With Most Total Factors", "Factorizations With Most Unique Factors");
    printRecordLists(mostFactors, mostUniqueFactors, 
        [](size_t i, const RankingList& leftList){ return std::format("#{}: {} | {}", i + 1, leftList.viewEntryAt(i).factorization.getFactorCount(), leftList.viewEntryAt(i).calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {} | {}", i + 1, rightList.viewEntryAt(i).factorization.getUniqueFactorCount(), rightList.viewEntryAt(i).calcTime); });

    //string stream used to format info blocks horizontally (to better fit in one screen)
    //various statistical facts regarding calculation times 
    printDivider("Calculation Times");
    std::println("{:{}}{}", 
        std::format("{}{}", "Q0: ", count ? fastest.viewEntryAt(0).calcTime : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
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
        std::format("{}{}", "Q4: ", slowest.viewEntryAt(0).calcTime), miniPanelWidth, 
        std::format("{}{}", "Standard Deviation: ", stdDev));
    //prints an overview of the calculation time distribution
    printDivider("Counts (fastest applicable category only)");
    categories.printout();
}

void StatSet::handleNewTime(factorizedNumInfo&& newFactorization) {
    //compares new item against each ranking list, inserting if and when appropriate
    fastest.checkAndRank(newFactorization);
    slowest.checkAndRank(newFactorization);
    mostFactors.checkAndRank(newFactorization);
    mostUniqueFactors.checkAndRank(newFactorization);
  
    //records the calculation time for calculation of a few statistical measures after completion
    timesData.emplace_back(newFactorization.calcTime);
}

void StatSet::completeFinalCalculations(void) {
    fullSequenceRunDuration = std::chrono::steady_clock::now() - start;

    //avoid division by 0, as well as attempted access at [-1]
    if (!count) return;

    //places the collected times in order
    std::sort(timesData.begin(), timesData.end());

    const auto findTimeAt = [this](double pos){ return timesData[(size_t)(pos)] * (1 - (pos - floor(pos))) + timesData[(size_t)(pos + (size_t)(timesData.size() > 1))] * (pos - floor(pos)); };
    median = findTimeAt((double)(timesData.size() - 1) / 2.);
    firstQuart = findTimeAt((double)(timesData.size() - 1) / 4.);
    thirdQuart = findTimeAt(((double)(timesData.size() - 1) * 3. / 4.));

    std::chrono::duration<long double, std::milli> interQuartileSum = timesData.size() % 4 / 4. * (timesData[timesData.size() / 4] + timesData[(timesData.size() - 1) - (timesData.size() / 4)]);

    //corrects special case where count == 1 causes the below to evaluate to -nan
    //calculates arithmetic mean to be used to find variances
    std::chrono::duration<long double, std::milli> sumTime(0);
    for (const std::chrono::duration<long double, std::milli>& time : timesData) sumTime += time;
    arithMean = sumTime / count;
    
    std::chrono::duration<long double, std::milli> sumReciprocals(0), sumLogs(0), sumSquaredDeviation(0);
    for (const std::chrono::duration<long double, std::milli> time : timesData) {
        categories.increment(time.count()); 
        //totalling of several sums to later be used in calculating respective means
        sumReciprocals      += std::chrono::duration<long double, std::milli>(1 / time.count());
        sumLogs             += std::chrono::duration<long double, std::milli>(logl(time.count()));
        sumSquaredDeviation += std::chrono::duration<long double, std::milli>(powl(time.count() - arithMean.count(), 2));

        //only adds to the interquartile total if within the inner quartiles
        if (time.count() > firstQuart.count() && time.count() < thirdQuart.count()) interQuartileSum += time;
    }
    //calculates final values of means (besides arithmetic, already calculated in preparation for stdDev calculation), as well as stdDev
    harmonMean = std::chrono::duration<long double, std::milli>(1 / (sumReciprocals / count).count());
    geoMean =    std::chrono::duration<long double, std::milli>(expl(sumLogs.count() / count));
    iqMean =     std::chrono::duration<long double, std::milli>(interQuartileSum * 2. / count);
    stdDev =     std::chrono::duration<long double, std::milli>(sqrtl(sumSquaredDeviation.count() / count));
}

void StatSet::initialize() {
    timesData.reserve(count);
    start = std::chrono::steady_clock::now();
}

void StatSet::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList) const {
    printRecordLists(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](size_t i, const RankingList& leftList){ return std::format("#{}: {}", i + 1, leftList.viewEntryAt(i).calcTime); },
        [](size_t i, const RankingList& rightList){ return std::format("#{}: {}", i + 1, rightList.viewEntryAt(i).calcTime); });
}

void StatSet::printRecordLists(const RankingList& leftRecordList, const RankingList& rightRecordList, std::function<const std::string(size_t index, const RankingList& list)>&& leftInfoFormat, std::function<const std::string(size_t index, const RankingList& list)>&& rightInfoFormat) const {
    for (size_t i = 0; i < std::min((unsigned long long)recordSize, count); ++i) {
        std::println("{:{}}{}\n{:{}}{}\n", 
            //info
            leftInfoFormat(i, leftRecordList), panelWidth,
            rightInfoFormat(i, rightRecordList),
            //factorizations themselves
            std::format("{} ={}", leftRecordList.viewEntryAt(i).n, leftRecordList.viewEntryAt(i).factorization.asString()), panelWidth,
            std::format("{} ={}", rightRecordList.viewEntryAt(i).n, rightRecordList.viewEntryAt(i).factorization.asString()));
    }
}