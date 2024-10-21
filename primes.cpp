#include "primes.hpp"

Factorization primes::primeFactorization(uint64_t n) {
    Factorization foundFactors;
    //counts powers of discovered prime factors
    //doubles as an flag of n's value being lowered since previous isPrime(n...) check, which results from said powers being factored out
    uint_fast8_t exp { 0 };
    //special case for multiples of nontrivial powers of 2
    //simplifies skipping evens for the rest of this instance of the function 
    if (n > 1ull) {
        for (; !(n & 0b1); ++exp) n >>= 1;
        if (exp) foundFactors.addNewFactor(2, exp);
    }
    //set to 1 to catch when n is already prime 
    exp = 1;
    //divides n by all odd primes until reaching the value of each of n's prime factors,
    //possibly excluding the greatest prime factor iff the square of said factor does not divide n
    uint64_t divisor { 1 };
    while (n > 1ull) {
        if (exp) { //only need to recheck if n has changed since last check 
            //n % k where 0 < k < divisor is already checked and can be skipped here, hence passing divisor as factor floor
            if (isPrime(n, divisor)) { 
                //no n^k where integer k != 1 is prime, therefore k == 1
                foundFactors.addNewFactor(n, 1);
                break; //n must be the largest prime factor at this point, therefore the loop may now be exited
            } 
            exp = 0; //exp no longer needs to act as a flag this iteration, resets to 0
        }
        //only relevant factors are primes
        //note that divisor is always incremented at least once
        while (!isPrime(divisor += 2u));

        for (; n % divisor == 0; ++exp) n /= divisor;
        if (exp) foundFactors.addNewFactor(divisor, exp);
    }
    return foundFactors;
}

inline bool primes::isPrime(const uint64_t n) {
    return isPrime(n, 5);
}

inline bool primes::isPrime(const uint64_t n, const uint64_t potentialFactorFloor) {
    //this switch is slightly faster than any obvious bitwise approaches tested so far, which all need to first check (n < 4)
    switch (n) {
        case 0ul: 
        case 1ul: return false;
        case 2ul: 
        case 3ul: return true;
    }

    //handles these two cases separately to allow optimizations in the loop below
    if (n % 2ul == 0ull || n % 3ul == 0ul) return false;

    //greatest integer <= the sqrt of n, truncation ok
    const uint32_t maxLessorDivisor { static_cast<uint32_t>(sqrt(n)) };

    //iterates through all odd numbers from at least 5 through the greatest odd int <= n's sqrt inclusive
    for (uint64_t i { std::max(potentialFactorFloor, 5ul) }; i <= maxLessorDivisor; i += 2ul) {
        //skip multiples of 3 (very common case); severely diminishing returns for numbers beyond this
        //additionally checking for 5 heuristically appears to slow runtime on average by ~1.25x, whereas the case with no such skips at all is ~1.5x
        if (i % 3ul == 0) i += 2ul;
        //if n is divisible by any of these values, n is not prime
        if (n % i == 0) return false;
    }
    return true;
}

std::unordered_set<uint32_t> primes::populatePrimeSet() {
    std::unordered_set<uint32_t> primeSet;
    //use no more than half the total available memory
    for (uint32_t i = 2; primeSet.size() < primeSet.max_size() / 2; ++i) 
        if (isPrime(i)) primeSet.insert(2);
    
    return primeSet;
}
