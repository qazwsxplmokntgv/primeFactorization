#pragma once

#include <cmath>
#include <cstdint>
#include <vector>
#include "utils.hpp"

//returns a map of prime factors of n and their respective powers in the form key == base, val == power
std::vector<factor> primeFactorization(unsigned long long n);

//if n is known to have no factors less than a certain number x, x can be passed in as the potentialFactorFloor
//returns whether n is prime or not
bool isPrime(unsigned long long n, unsigned long long potentialFactorFloor = 2);
