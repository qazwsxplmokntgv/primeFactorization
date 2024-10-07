#include "factorizationcalculator.hpp"

FactorizationCalculator::FactorizationCalculator() {
    //TODO add option for saving and loading settings from file
    promptForMode();

    //WIP
    //if ('y' == std::tolower(promptIndividualSetting<char>("Load settings from file? (y/n): ", [](char input){ return tolower(input) == 'y' || tolower(input) == 'n'; }))) 

    promptForSettings();
    stats.emplace(inputCount);
}

void FactorizationCalculator::run(void) {

    std::println("\n\n");

    auto start = std::chrono::steady_clock::now();

    switch (mode) {
    case InputMode::MANUAL:
        manualInputTest();
        break;
    case InputMode::RANDOM:
        randomInputTest();
        break;
    case InputMode::RANGE:
        rangeBasedInputTest();
        break;
    }

    std::chrono::duration<long double> executionTime = std::chrono::steady_clock::now() - start;

    stats->completeFinalCalculations();
    //stat printout header
    printDivider();
    //if minN/maxN were unset/irrelevant (e.g. in manual mode), omit that information
    std::print("{} factorizations{} calculated in {}.\n", inputCount, maxN ? std::format(" of numbers{} <= {}", (minN ? std::format(" >= {} and", minN) : ""), maxN) : "", executionTime);
    
    stats->printout();
}

void FactorizationCalculator::promptForMode(void) {
    //converts user input int to an InputMode
    mode = static_cast<InputMode>(promptIndividualSetting<int>("Input Mode:\n[1]Manual\n[2]Random\n[3]Range\n", [](int input){ return input > 0 && input <= modeCount; }) - 1);
}

void FactorizationCalculator::promptForSettings(void) {

    //prompts user for mode relevant settings
    if (mode == InputMode::MANUAL || mode == InputMode::RANDOM)
        inputCount = promptIndividualSetting<uint64_t>("Count: ");
    if (mode == InputMode::RANGE) 
        minN = promptIndividualSetting<uint64_t>("Lower Bound: "); //while applicable to random, generally found to be less useful than annoying
    if (mode == InputMode::RANDOM || mode == InputMode::RANGE) {
        maxN = promptIndividualSetting<uint64_t>("Upper Bound (0 for max): ");
        reportIndividualFactorizations = 'y' == std::tolower(promptIndividualSetting<char>("Report Individual Factorizations? (y/n): ", [](char input){ return tolower(input) == 'y' || tolower(input) == 'n'; }));
        if (!maxN) maxN = std::numeric_limits<uint64_t>::max();
    }

    //TODO allow settings to be saved per mode here

    //defaults or extrapolates remaining settings
    if (mode == InputMode::MANUAL) {
        minN = maxN = 0; //indicates unset
        reportIndividualFactorizations = true;
    }
    if (mode == InputMode::RANDOM)
        minN = 0;
    if (mode == InputMode::RANGE) 
        inputCount = (maxN - minN) + 1;
}

void FactorizationCalculator::manualInputTest() {
    for (uint64_t i { 1 }; i <= inputCount; ++i) {
        FactorCalculationInfo infoSet; 

        std::print("({}/{}) Num: ", i, inputCount);
        std::cin >> infoSet.n;
        
        //finds n's factorization, timing the operation
        infoSet.calculateAndTime();

        //displays factorization and respective calculation time
        infoSet.printPostCalcInfo();

        //passes info along to calculate stats shown on program conclusion
        stats->handleNewTime(std::move(infoSet));
    }
}

void FactorizationCalculator::randomInputTest() {
    //set up prng machine
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<uint64_t> flatDistr(0, maxN);

    for (uint64_t i { 1 }; i <= inputCount; ++i) {
        //generate a number
        FactorCalculationInfo infoSet { flatDistr(gen) };

        if (reportIndividualFactorizations) //display the number generated
            std::println("({}/{}): {}", i, inputCount, infoSet.n);
        else if ((100 * i / inputCount != 100 * (i - 1) / inputCount) || i == 1) //display progress through count as a % 
            std::println("\033[A\33[2K\r{}%", 100 * i / inputCount);

        infoSet.calculateAndTime();

        //prints out the individual factorization and respective calculation time
        if (reportIndividualFactorizations) infoSet.printPostCalcInfo();

        stats->handleNewTime(std::move(infoSet));
    }
}

void FactorizationCalculator::rangeBasedInputTest() {
    for (uint64_t i { 1 }; i <= inputCount; ++i) {
        //generate a number
        FactorCalculationInfo infoSet { (i - 1) + minN };

        if (reportIndividualFactorizations) //display the number generated
            std::println("({}/{}): {}", i, inputCount, infoSet.n);
        else if (100 * i / inputCount != 100 * (i - 1) / inputCount || i == 1) //display progress through count as a % 
            std::println("\033[A\33[2K\r{}%", 100 * i / inputCount);

        infoSet.calculateAndTime();

        //prints out the individual factorization and respective calculation time
        if (reportIndividualFactorizations) infoSet.printPostCalcInfo();

        stats->handleNewTime(std::move(infoSet));
    }
}
