#include "sequence.hpp"

void sequence::findUserRequestedFactorizations(unsigned long long count) {
    statCollection stats(count, std::max((unsigned int)log10l(count), 3u), 0);
    for (unsigned long long i = 1; i <= count; ++i) {
        factorizedNumInfo infoSet; 

        std::cout << '(' << i << '/' << count << ')' << " Num: ";
        std::cin >> infoSet.n;
        
        //times the operation
        auto start = std::chrono::steady_clock::now();
        infoSet.factorization = primes::primeFactorization(infoSet.n);
        infoSet.calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);

        printFactorization(infoSet.factorization);
        //displays time taken to find the factorization
        std::cout << '\n' << infoSet.calcTime.count() << "ms\n\n";

        stats.handleNewTime(std::move(infoSet));
    }
    stats.completeFinalCalculations();
    stats.printout();
}

void sequence::testRandomNumberFactorizations(unsigned long long count, const bool shouldReportEachFactorization, const unsigned long long maxN) {
    //set up prng machine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long long> flatDistr(0, maxN);

    //collection of stats from upcoming calculation time data
    //stores a flexible number of records in a few categories based on the log of the count, with a minimum of 3
    statCollection stats(count, std::max((unsigned int)log10l(count), 3u), maxN);

    for (unsigned long long i = 0; i < count; ++i) {
        //generate a number
        factorizedNumInfo infoSet { flatDistr(gen) };

        if (shouldReportEachFactorization) //display the number generated
            std::cout << '(' << i + 1 << '/' << count << ')' << ": " << infoSet.n << std::endl;
        else if (100 * i / count != 100 * (i - 1) / count) //display progress through count as a % 
            std::cout << "\033[A\33[2K\r" << 100 * i / count << "%\n";

        //times the operation
        auto start = std::chrono::steady_clock::now();
        infoSet.factorization = primes::primeFactorization(infoSet.n);
        infoSet.calcTime = std::chrono::duration<long double, std::milli>(std::chrono::steady_clock::now() - start);

        //prints out the individual factorization and respective calculation time
        if (shouldReportEachFactorization) {
            printFactorization(infoSet.factorization);
            std::cout << '\n' << infoSet.calcTime.count() << "ms\n\n";
        }
        stats.handleNewTime(std::move(infoSet));
    }
    stats.completeFinalCalculations();
    stats.printout();
}

