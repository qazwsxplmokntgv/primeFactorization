#include "stats.hpp"

statCollection::statCollection(const unsigned long long inputCount, const size_t recordCount, const unsigned long long maxInput) : count(inputCount), maxN(maxInput) {
    fastest.resize(recordCount);
    slowest.resize(recordCount);
    mostFactors.resize(recordCount);
    timesData.reserve(count);
    for (factorizedNumInfo& info : fastest) info.calcTime = std::chrono::duration<long double, std::milli>(std::numeric_limits<long double>::max());
    start = std::chrono::steady_clock::now();
}

void statCollection::printout(void) const {
    constexpr int rightPanelOffset = 64;

    std::cout << "\033[1;4;97m--------------------------------------------------------------------------------------------------------------------------------\033[0m\n";
    std::cout << count << " factorizations of numbers <= " << maxN << " calculated in " << fullSequenceRunDuration.count() << " seconds.\n\n";

    std::stringstream str; 
    std::cout << "\033[1;4;97m---Fastest Factorizations Attempted-----------------------------\033[0m";
    std::cout << "\033[1;4;97m---Slowest Factorizations Attempted-----------------------------\033[0m\n";
    for (size_t i = 0; i < fastest.size() && fastest[i].calcTime < std::chrono::duration<long double, std::milli>(std::numeric_limits<long double>::max()); ++i) {
        str << '#' << i + 1 << ": " << fastest[i].calcTime.count() << "ms";
        std::cout << std::left << std::setw(rightPanelOffset) << str.str() << '#' << i + 1 << ": " << slowest[i].calcTime.count() << "ms\n";
        str.str("");

        str << fastest[i].n << " =";
        printFactorization(fastest[i].factorization, str);
        std::cout << std::setw(rightPanelOffset) << str.str() << slowest[i].n << " =";
        printFactorization(slowest[i].factorization);
        std::cout << "\n\n";
        str.str("");
    }

    std::cout << "\033[1;4;97m---Factorizations With Most Factors-----------------------------\033[0m";
    std::cout << "\033[1;4;97m---Mysterious Fourth Thing--------------------------------------\033[0m\n";
    for (size_t i = 0; i < mostFactors.size() && mostFactors[i].n; ++i) {
        std::cout << '#' << i + 1 << ": " << getFactorCount(mostFactors[i].factorization) << " | " << mostFactors[i].calcTime.count() << "ms\n";

        std::cout << mostFactors[i].n << " =";
        printFactorization(mostFactors[i].factorization);
        std::cout << "\n\n";
    }

    std::cout << "\033[1;4;97m---Calculation Times------------------------------------------------------------------------------------------------------------\033[0m\n";
    str << fastest[0].calcTime.count() << "ms";
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

    categories.print();
}

void statCollection::noteNewTime(const factorizedNumInfo& newFactorization) {

    rankIfApplicable(newFactorization, fastest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return newItem.calcTime.count() < existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, slowest, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return newItem.calcTime.count() > existingItem.calcTime.count(); });
    rankIfApplicable(newFactorization, mostFactors, [](const factorizedNumInfo& newItem, const factorizedNumInfo& existingItem){ return getFactorCount(newItem.factorization) > getFactorCount(existingItem.factorization); });

    //totals a running sum of times
    runningSum += newFactorization.calcTime;

    //records the calculation time for calculation of a few statistical measures after completion
    timesData.push_back(newFactorization.calcTime);
}

void statCollection::completeFinalCalculations(void) {
    fullSequenceRunDuration = std::chrono::steady_clock::now() - start;

    if (!count) return;

    //places the collected times in order
    std::sort(timesData.begin(), timesData.end());

    std::chrono::duration<long double, std::milli> interQuartileSum;
    //i am sure there was a better, easier way. but this does work
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

    //calculates arithmetic mean to be used to find variances
    arithMean = runningSum / count;
    
    std::chrono::duration<long double, std::milli> sumReciprocals(0), sumLogs(0), sumSquaredDeviation(0);
    for (const std::chrono::duration<long double, std::milli> time : timesData) {
        categories.increment(time.count()); //to be optimized
        //totalling of several sums to later be used in calculating respective means
        sumReciprocals += std::chrono::duration<long double, std::milli>(1 / time.count());
        sumLogs += std::chrono::duration<long double, std::milli>(logl(time.count()));
        if (time.count() > firstQuart.count() && time.count() < thirdQuart.count()) interQuartileSum += time;
        sumSquaredDeviation += std::chrono::duration<long double, std::milli>(powl(time.count() - arithMean.count(), 2));
    }
    //calculates final values of means (besides arithmetic, already calculated in preparation for stdDev calculation), as well as stdDev
    harmonMean = std::chrono::duration<long double, std::milli>(1 / (sumReciprocals / count).count());
    geoMean = std::chrono::duration<long double, std::milli>(expl(sumLogs.count() / count));
    iqMean = std::chrono::duration<long double, std::milli>(interQuartileSum * 2. / count);
    stdDev = std::chrono::duration<long double, std::milli>(sqrtl(sumSquaredDeviation.count() / count));
}
