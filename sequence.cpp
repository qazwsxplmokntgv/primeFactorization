#include "sequence.hpp"

void sequence::findUserRequestedFactorizations(unsigned long long count) {
    StatSet stats(count, std::max((unsigned int)log10l(count), 3u), 0);
    for (unsigned long long i = 1; i <= count; ++i) {
        factorizedNumInfo infoSet; 

        std::print("({}/{}) Num: ", i, count);
        std::cin >> infoSet.n;
        
        //times the operation
        auto start = std::chrono::steady_clock::now();
        infoSet.factorization = primes::primeFactorization(infoSet.n);
        infoSet.calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);

        //displays factorization and respective calculation time
        std::println("{}\n{}\n", infoSet.factorization.asString(), infoSet.calcTime);

        //passes info along to calculate stats shown on program conclusion
        stats.handleNewTime(std::move(infoSet));
    }
    stats.completeFinalCalculations();
    stats.printout();
}

void sequence::testRandomNumberFactorizations(unsigned long long count, const bool shouldReportEachFactorization, const unsigned long long maxN) {
    //set up prng machine
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<unsigned long long> flatDistr(0, maxN);

    //collection of stats from upcoming calculation time data
    //stores a flexible number of records in a few categories based on the log of the count, with a minimum of 3
    //also starts the programs overall runtime timer
    StatSet stats(count, std::max((unsigned int)log10l(count), 3u), maxN);

    for (unsigned long long i = 1; i <= count; ++i) {
        //generate a number
        factorizedNumInfo infoSet { flatDistr(gen) };

        if (shouldReportEachFactorization) //display the number generated
            std::println("({}/{}): {}", i, count, infoSet.n);
        else if (100 * i / count != 100 * (i - 1) / count || i == 1) //display progress through count as a % 
            std::print("\033[A\33[2K\r{}%", 100 * i / count);

        //times the operation
        auto start = std::chrono::steady_clock::now();
        infoSet.factorization = primes::primeFactorization(infoSet.n);
        infoSet.calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);

        //prints out the individual factorization and respective calculation time
        if (shouldReportEachFactorization) 
            std::println("{}\n{}\n", infoSet.factorization.asString(), infoSet.calcTime);

        stats.handleNewTime(std::move(infoSet));
    }
    stats.completeFinalCalculations();
    stats.printout();
}
