#pragma once

#include <cmath>
#include <cstdint>
#include <vector>
#include "utils.hpp"

//returns a map of prime factors of n and their respective powers in the form key == base, val == power
std::vector<factor> primeFactorization(unsigned long long n);

//returns whether n is prime or not
inline bool isPrime(unsigned long long n);
//if n is known to have no factors less than a certain number, that number can be passed in as the potentialFactorFloor
inline bool isPrime(unsigned long long n, unsigned long long potentialFactorFloor);
