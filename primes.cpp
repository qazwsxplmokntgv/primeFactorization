#include "primes.hpp"

Factorization primes::primeFactorization(unsigned long long n) {
    Factorization basePowers;

    //counts powers of discovered prime factors
    //doubles as an flag of n's value being lowered since previous isPrime(n...) check, which results from said powers being factored out
    //initially 1 to catch when original input is already prime
    uint_fast8_t exp = 0;

    //special case for multiples of nontrivial powers of 2
    //allows evens to be cleanly skipped in the rest of the function
    if (n > 1u) {
        for (; !(n & 0b1); ++exp) n >>= 1;
        if (exp) basePowers.addNewFactor(2u, exp);
    }
    exp = 1;
    //sweeps across all odd primes until reaching the value of each of n's prime factors,
    //possibly excluding the greatest prime factor as long as the square of said factor does not divide n
    unsigned long long divisor = 1;

    while (n > 1u) {
        //checking if n itself is prime
        if (exp) { //only rechecks if n has changed since last check 
            //skips redundant checking for factors of n less than divisor (already checked in second inner while loop)
            if (isPrime(n, divisor)) { 
                //no n^k where k != 1 is prime, therefore k == 1
                basePowers.addNewFactor(n, 1);
                //n must be the largest prime factor at this point, therefore the loop may now be exited
                break;
            } 
            exp = 0; //resets exp following its use as a flag for unchecked modified n values
        }
        //skip non primes before checking for factor status
        //divisor is always incremented at least once
        while (!isPrime(divisor += 2u));
        //determine the greatest power of divisor by which n is divisible by, if any
        while (n % divisor == 0) {
            n /= divisor;
            ++exp;
        }
        //if a non 0 power of i is a factor of n, add this to the output and move on
        if (exp) basePowers.addNewFactor(divisor, exp);
    }
    return basePowers;
}

inline bool primes::isPrime(const unsigned long long n) {
    return isPrime(n, 5);
}

//O(sqrt(n))
//Ω(1)
inline bool primes::isPrime(const unsigned long long n, const unsigned long long potentialPrimeFactorFloor) {
    //this switch is slightly faster than any obvious bitwise approaches tested so far, which all need to first check (n < 4)
    switch (n) {
        case 0: 
        case 1: return false;
        case 2: 
        case 3: return true;
    }

    //handles these two cases separately to allow optimizations in the for loop below
    if (n % 2u == 0 || n % 3u == 0) return false;

    //greatest integer <= the sqrt of n
    //sqrt has sufficient precision, sqrtl not necessary
    const unsigned long maxLessorDivisor = sqrt(n);

    //iterates through all odd numbers from at least 5 through the greatest odd int <= n's sqrt inclusive
    for (unsigned long long i = std::max(potentialPrimeFactorFloor, 5ull); i <= maxLessorDivisor; i += 2u) {
        //skip multiples of 3 (very common case); severely diminishing returns for numbers beyond this
        //additionally checking for 5 heuristically appears to slow runtime on average by about 1.5x, slightly worse than case with no such skips at all (~1.4x)
        if (i % 3u == 0) i += 2u;
        //if n is divisible by any of these values, n is not prime
        if (n % i == 0) return false;
    }
    return true;
}

// inline bool primes::isPrimeRemembers(const unsigned long long n) {
//     return isPrimeRemembers(n, 5);
// }

// inline bool primes::isPrimeRemembers(const unsigned long long n, const unsigned long long potentialPrimeFactorFloor) {
//     if (knownNonPrimes.contains(n)) return false;
//     if (!isPrime(n, potentialPrimeFactorFloor)) {
//         knownNonPrimes.insert(n);
//         knownNonPrimes.insert(n >> 1);
//         knownNonPrimes.insert(n >> 2);
//         return false; 
//     }
//     return true;
// }
