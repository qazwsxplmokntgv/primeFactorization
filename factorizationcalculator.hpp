#pragma once

#include <cstdint>
#include <chrono>
#include <functional>
#include <limits>
#include <iostream>
#include <print>
#include <string>
#include <random>
#include <optional>
#include <unordered_set>
#include "statset.hpp"
#include "primes.hpp"
#include "calculationinfo.hpp"

static constexpr int modeCount = 3;

enum class InputMode {
    MANUAL, RANDOM, RANGE
};

class FactorizationCalculator {
public:
    FactorizationCalculator();
    void run(void);
private:
    //prompts the user to select a mode for the calculator to run in
    void promptForMode(void);

    //presents a series of prompts to the user, enabling them to set the calculator's settings
    //precondition: mode is set
    void promptForSettings(void);

    //allows the user to input inputCount individual numbers to be processed
    void manualInputTest();

    //randomly generates and processes inputCount numbers below maxN
    void randomInputTest();

    //inputs every value from minN to maxN in order
    void rangeBasedInputTest();

    InputMode mode;
    uint64_t inputCount, minN, maxN;
    bool reportIndividualFactorizations;
    
    //collection of stats from upcoming calculation time data
    //stores a flexible number of records in a few timeCategories based on the log of the count, with a minimum of 3
    //optional to postpone construction until settings have been set
    std::optional<StatSet> stats;
};

//accept any valid input that can be stored in type T unless otherwise specified
//acceptCondition must evalutate true for the input to be accepted
template<typename T>
inline T promptIndividualSetting(std::string&& prompt, const std::function<bool(T)> acceptCondition = [](T setting){ return true; }) {
    T setting;
    do {
        std::print("{}", prompt);
        while (!(std::cin >> setting)) {
            std::print("\033[A\33[2K\r{}", prompt);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (!acceptCondition(setting));
    return setting;
}

//n assumed to start at 1
inline bool yieldsNewIntegerPercentage(uint64_t n, uint64_t total);