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
    std::cout << count << " factorizations";
    //0 maxN is used to indicate manual entry - the upper rng bound is accordingly omitted
    if (maxN) std::cout << " of numbers <= " << maxN; 
    std::cout << " calculated in " << fullSequenceRunDuration.count() << " seconds.\n\n";

    //info blocks on the fastest and slowest calculation times for factorizations
    //shows calc time, the input, and the factorization itself
    printDivider("Fastest Factorizations Attempted", "Slowest Factorizations Attempted");
    printRecordList(fastest, slowest);

    //as above, for factorizations with the most factors
    printDivider("Factorizations With Most Factors", "Mysterious Fourth Thing");
    printRecordList(mostFactors);

    //string stream used to format info blocks horizontally (to better fit in one screen)
    std::stringstream str; 
    //various statistical facts regarding calculation times 
    printDivider("Calculation Times");
    str << (count ? fastest[0].calcTime.count() : 0) << "ms";
    std::cout << "Q0: " << std::setw(20) << str.str() << "Harmonic Mean:      " << harmonMean.count() << "ms\n";
    str.str("");
    str << firstQuart.count() << "ms";
    std::cout << "Q1: " << std::setw(20) << str.str() << "Geometric Mean:     " << geoMean.count() << "ms\n";
    str.str("");
    str << median.count() << "ms";
    std::cout << "Q2: " << std::setw(20) << str.str() << "Interquartile Mean: " << iqMean.count() << "ms\n";
    str.str("");
    str << thirdQuart.count() << "ms";
    std::cout << "Q3: " << std::setw(20) << str.str() << "Arithmetic Mean:    " << arithMean.count() << "ms\n";
    str.str("");
    str << slowest[0].calcTime.count() << "ms";
    std::cout << "Q4: " << std::setw(20) << str.str() << "Standard Deviation: " << stdDev.count() << "ms\n\n";

    //prints an overview of the calculation time distribution
    categories.print();
}

void statCollection::handleNewTime(factorizedNumInfo&& newFactorization) {

    //checks if the new time is elligible to join any of the rankings being tracked
    rankIfApplicable(newFactorization, fastest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return newItem.calcTime.count() < existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, slowest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return newItem.calcTime.count() > existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, mostFactors, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return getFactorCount(newItem.factorization) > getFactorCount(existingItem.factorization); });

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
    fastest.resize(recordCount, { 0ull, std::vector<factor>(), std::chrono::duration<long double, std::milli>(std::numeric_limits<long double>::max()) });
    slowest.resize(recordCount);
    mostFactors.resize(recordCount);
    timesData.reserve(count);
    start = std::chrono::steady_clock::now();
}

void statCollection::printRecordList(const std::vector<factorizedNumInfo>& recordList) const {
    for (size_t i = 0; i < recordList.size() && recordList[i].n; ++i) {
        std::cout << '#' << i + 1 << ": " << getFactorCount(recordList[i].factorization) << " | " << recordList[i].calcTime.count() << "ms\n";

        std::cout << recordList[i].n << " =";
        printFactorization(recordList[i].factorization);
        std::cout << "\n\n";
    }
}

void statCollection::printRecordList(const std::vector<factorizedNumInfo>& leftRecordList, const std::vector<factorizedNumInfo>& rightRecordList) const {
    std::stringstream str; 
    for (size_t i = 0; i < std::min((unsigned long long)recordSize, count); ++i) {
        //format is "ranking: calculation time\n"
        str << '#' << i + 1 << ": " << leftRecordList[i].calcTime.count() << "ms";
        //adds space between left and right panels
        std::cout << std::left << std::setw(panelWidth) << str.str() << '#' << i + 1 << ": " << rightRecordList[i].calcTime.count() << "ms\n";
        str.str("");

        //format = "input == factorization\n\n"
        //left side
        str << leftRecordList[i].n << " =";
        printFactorization(leftRecordList[i].factorization, str);
        //right side
        std::cout << std::setw(panelWidth) << str.str() << rightRecordList[i].n << " =";
        printFactorization(rightRecordList[i].factorization);
        std::cout << "\n\n";
        str.str("");
    }
}
