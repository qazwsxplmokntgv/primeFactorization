#include "primes.hpp"

std::vector<factor> primeFactorization(unsigned long long n) {
    std::vector<factor> basePowers;
    basePowers.reserve(4); //enough for a solid portion of factorizations 

    //special case for multiples of nontrivial powers of 2
    //allows evens to be cleanly skipped in the rest of the function
    if (n > 1u) {
        uint_fast8_t twosCount = 0;
        for (; !(n & 0b1); ++twosCount) n >>= 1;
        if (twosCount) basePowers.emplace_back(2u, twosCount);
    }

    //value which sweeps across all odd primes until reaching the value of each of n's prime factors,
    //possibly excluding the greatest prime factor if the square of said factor does not divide n
    unsigned long long divisor = 1;
    //counts powers of discovered prime factors
    //doubles as an indicator of n's value being lowered since last time n's primeness was checked, which results from said powers being factored out
    //initially 1 to catch when original input is already prime
    uint_fast8_t exp = 1;

    while (n > 1u) {
        //checking if n itself is prime
        if (exp) { //only rechecks if n has changed since last check 
            //skips redundant checking for factors of n less than divisor (already checked in second inner while loop)
            if (isPrime(n, divisor)) { 
                //if n is prime, n^k where k > 1 cannot be a factor
                basePowers.emplace_back(n, 1);
                //n must be the largest prime factor at this point, therefore the loop may now be exited
                break;
            } //resets exp following its use as a flag for unchecked modified n values
            exp = 0;
        }
        //skip non primes before checking for factor status
        while (!isPrime(divisor += 2u));
        //determine the greatest power of divisor by which n is divisible by, if any
        while (n % divisor == 0) {
            n /= divisor;
            ++exp;
        }
        //if a non 0 power of i is a factor of n, add this to the output and move on
        if (exp) basePowers.emplace_back(divisor, exp);
    }
    return basePowers;
}

inline bool isPrime(unsigned long long n) {
    return isPrime(n, 5);
}

inline bool isPrime(unsigned long long n, unsigned long long potentialFactorFloor) {
    //this switch is slightly faster than any obvious bitwise approaches tested so far, which all need to first check (n < 4)
    switch (n) {
        [[unlikely]] case 0u: 
        [[unlikely]] case 1u: return false;
        case 2u: 
        case 3u: return true;
    }

    //handles these two cases separately to allow optimizations in the for loop below
    if (n % 2u == 0 || n % 3u == 0) return false;

    //greatest integer <= the sqrt of n
    const unsigned long maxLessorDivisor = sqrt(n);

    //iterates through all odd numbers from at least 5 through the greatest odd int <= n's sqrt inclusive
    for (unsigned long long i = potentialFactorFloor > 5u ? potentialFactorFloor : 5; i <= maxLessorDivisor; i += 2u) {
        //skip multiples of 3 (very common case); severely diminishing returns for numbers beyond this
        //additionally checking for 5 heuristically appears to slow runtime on average by about 1.5x, slightly worse than case with no such skips at all (~1.4x)
        if (i % 3 == 0) i += 2u;
        //if n is divisible by any of these values, n is not prime
        if (n % i == 0) return false;
    }
    return true;
}
