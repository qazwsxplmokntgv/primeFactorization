#include "stats.hpp"

statCollection::statCollection(const unsigned long long inputCount, const size_t recordCount, const unsigned long long maxInput) 
    : count(inputCount), recordSize(recordCount), maxN(maxInput) {
    initialize(recordCount);
}

statCollection::statCollection(const unsigned long long inputCount, const size_t recordCount) 
    : count(inputCount), recordSize(recordCount), maxN(0) {
    initialize(recordCount);
}

void statCollection::printout(void) const {
    //stat printout header
    printDivider();
    std::cout << std::format("{} factorizations{} calculated in {}.\n\n", count, maxN ? std::format(" of numbers <= {}", maxN) : "", fullSequenceRunDuration);

    //info blocks on the fastest and slowest calculation times for factorizations
    //shows calc time, the input, and the factorization itself
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted");
    printRecordList(fastest, slowest);

    //as above, for factorizations with the most factors
    printDivider("Factorizations With Most Total Factors", "Factorizations With Most Unique Factors");
    

    printRecordList(mostFactors, mostUniqueFactors, 
        [](size_t i, std::vector<factorizedNumInfo> leftList){ return std::format("#{}: {} | {}", i + 1, leftList[i].factorization.getFactorCount(), leftList[i].calcTime); },
        [](size_t i, std::vector<factorizedNumInfo> rightList){ return std::format("#{}: {} | {}", i + 1, rightList[i].factorization.getUniqueFactorCount(), rightList[i].calcTime); });

    //string stream used to format info blocks horizontally (to better fit in one screen)
    //various statistical facts regarding calculation times 
    printDivider("Calculation Times");
    std::cout << std::format("{}{}{}{}{}\n", 
        std::format("{:{}}{}\n", 
            std::format("{}{}", "Q0: ", count ? fastest[0].calcTime : std::chrono::duration<long double, std::milli>(0)), miniPanelWidth, 
            std::format("{}{}", "Harmonic Mean:      ", harmonMean)),
        std::format("{:{}}{}\n", 
            std::format("{}{}", "Q1: ", firstQuart), miniPanelWidth, 
            std::format("{}{}", "Geometric Mean:     ", geoMean)),
        std::format("{:{}}{}\n", 
            std::format("{}{}", "Q2: ", median), miniPanelWidth, 
            std::format("{}{}", "Interquartile Mean: ", iqMean)),
        std::format("{:{}}{}\n", 
            std::format("{}{}", "Q3: ", thirdQuart), miniPanelWidth, 
            std::format("{}{}", "Arithmetic Mean:    ", arithMean)),
        std::format("{:{}}{}\n", 
            std::format("{}{}", "Q4: ", slowest[0].calcTime), miniPanelWidth, 
            std::format("{}{}", "Standard Deviation: ", stdDev))
    );
    //prints an overview of the calculation time distribution
    categories.printout();
}

void statCollection::handleNewTime(factorizedNumInfo&& newFactorization) {

    //checks if the new time is elligible to join any of the rankings being tracked
    rankIfApplicable(newFactorization, fastest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem)
        { return newItem.calcTime.count() < existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, slowest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem)
        { return newItem.calcTime.count() > existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, mostFactors, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem)
        //tie breaks with unique factors
        { return newItem.factorization.getFactorCount() > existingItem.factorization.getFactorCount() || (newItem.factorization.getFactorCount() == existingItem.factorization.getFactorCount() && newItem.factorization.getUniqueFactorCount() > existingItem.factorization.getUniqueFactorCount()); });
    rankIfApplicable(newFactorization, mostUniqueFactors, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem)
        //tie breaks with total factors
        { return newItem.factorization.getUniqueFactorCount() > existingItem.factorization.getUniqueFactorCount() || (newItem.factorization.getUniqueFactorCount() == existingItem.factorization.getUniqueFactorCount() && newItem.factorization.getFactorCount() > existingItem.factorization.getFactorCount()); });

    //totals a running sum of times for later use in calculating and average (used in turn for calculating deviations, variance, std deviation)
    runningSum += newFactorization.calcTime;

    //records the calculation time for calculation of a few statistical measures after completion
    timesData.push_back(newFactorization.calcTime);
}

void statCollection::completeFinalCalculations(void) {
    fullSequenceRunDuration = std::chrono::steady_clock::now() - start;

    //avoid division by 0, as well as attempted access at [-1]
    if (!count) return;

    //places the collected times in order
    std::sort(timesData.begin(), timesData.end());

    std::chrono::duration<long double, std::milli> interQuartileSum;
    switch (count % 4) {
        case 0:  
            median = (timesData[count / 2] + timesData[count / 2 - 1]) / 2.;
            firstQuart = (timesData[(count / 4) - 1] + timesData[count / 4]) / 2.;
            thirdQuart = (timesData[(count * 3 / 4) - 1] + timesData[count * 3 / 4]) / 2.;
            interQuartileSum = std::chrono::duration<long double, std::milli>(0);
            break;
        case 1:
            median = timesData[count / 2];
            firstQuart = ((timesData[(count / 4) - 1] * 3.) + timesData[count / 4]) / 4.;
            thirdQuart = (timesData[count * 3 / 4] + (timesData[(count * 3 / 4) + 1] * 3.)) / 4.;
            interQuartileSum = (timesData[(count / 4) - 1] + timesData[(count * 3 / 4) + 1]) * 3. / 4.;
            break;
        case 2: 
            median = (timesData[count / 2] + timesData[count / 2 - 1]) / 2.;
            firstQuart = timesData[count / 4];
            thirdQuart = timesData[3 * count / 4];
            interQuartileSum = (firstQuart + thirdQuart) / 2.;
            break;
        case 3: 
            median = timesData[count / 2];
            firstQuart = (timesData[count / 4] + (timesData[(count / 4) + 1] * 3.)) / 4.;
            thirdQuart = ((timesData[(3 * count / 4) - 1] * 3.) + timesData[3 * count / 4]) / 4.;
            interQuartileSum = (timesData[count / 4] + timesData[3 * count / 4]) / 4.;
            break;
    }
    //corrects special case where count == 1 causes the below to evaluate to -nan
    if (count == 1) { 
        firstQuart = thirdQuart = timesData[0];
        interQuartileSum = timesData[0] / 2.;
    }
    //calculates arithmetic mean to be used to find variances
    arithMean = runningSum / count;
    
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

void statCollection::initialize(const size_t recordCount) {
    fastest.resize(recordCount, { 0ull, {}, std::chrono::duration<long double, std::milli>(std::numeric_limits<long double>::max()) });
    slowest.resize(recordCount);
    mostFactors.resize(recordCount);
    mostUniqueFactors.resize(recordCount);
    timesData.reserve(count);
    start = std::chrono::steady_clock::now();
}

void statCollection::printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList) const {
    printRecordList(leftRecordList, rightRecordList, 
        //default format shows rank and calcTime only
        [](size_t i, std::vector<factorizedNumInfo> leftList){ return std::format("#{}: {}", i + 1, leftList[i].calcTime); },
        [](size_t i, std::vector<factorizedNumInfo> rightList){ return std::format("#{}: {}", i + 1, rightList[i].calcTime); });
}

void statCollection::printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList, std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)>&& leftInfoFormat, std::function<const std::string(size_t index, const std::vector<factorizedNumInfo>& list)>&& rightInfoFormat) const {
    for (size_t i = 0; i < std::min((unsigned long long)recordSize, count); ++i) {
        std::cout << std::format("{:{}}{}\n{:{}}{}\n\n", 
            //info
            leftInfoFormat(i, leftRecordList), panelWidth,
            rightInfoFormat(i, rightRecordList),
            //factorizations themselves
            std::format("{} ={}", leftRecordList[i].n, leftRecordList[i].factorization.asString()), panelWidth,
            (rightRecordList.size() > i) ? 
            std::format("{} ={}", rightRecordList[i].n, rightRecordList[i].factorization.asString()) : "");
    }
}

void rankIfApplicable(const factorizedNumInfo& newItem, std::vector<factorizedNumInfo>& existingRankings, const std::function<bool(const factorizedNumInfo&, const factorizedNumInfo&)>&& comparison) {
    //for each stored value, 
    for (size_t i = 0; i < existingRankings.size(); ++i) { 
        //find if the new factorization outranks any existing ranked factorizations
        if (comparison(newItem, existingRankings[i])) {
            //shifts indexes at or after i to the right, discarding the lowest ranked value
            for (size_t j = existingRankings.size() - 1; j > i; --j) {
                existingRankings[j] = existingRankings[j - 1];
            }
            //places the new factorization into the opened slot
            existingRankings[i] = newItem;
            break;
        }
    }
}
