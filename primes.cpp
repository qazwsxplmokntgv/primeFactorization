#include "primes.hpp"

Factorization primes::primeFactorization(unsigned long long n) {
    Factorization foundFactors;
    //counts powers of discovered prime factors
    //doubles as an flag of n's value being lowered since previous isPrime(n...) check, which results from said powers being factored out
    uint_fast8_t exp { 0 };
    //special case for multiples of nontrivial powers of 2
    //allows evens to be cleanly skipped in the rest of the function
    if (n > 1ull) {
        for (; !(n & 0b1); ++exp) n >>= 1;
        if (exp) foundFactors.addNewFactor(2, exp);
    }
    //set to 1 to catch when original input (* 2^exp) is already prime 
    exp = 1;
    //sweeps across all odd primes until reaching the value of each of n's prime factors,
    //possibly excluding the greatest prime factor iff the square of said factor does not divide n
    unsigned long long divisor { 1 };
    while (n > 1ull) {
        //checking if the remaining n is prime
        if (exp) { //only rechecks if n has changed since last check 
            //n % k where 0 < k < divisor is already checked and can be skipped here, hence passing divisor as factor floor
            if (isPrime(n, divisor)) { 
                //no n^k where integer k != 1 is prime, therefore k == 1
                foundFactors.addNewFactor(n, 1);
                break; //n must be the largest prime factor at this point, therefore the loop may now be exited
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
        if (exp) foundFactors.addNewFactor(divisor, exp);
    }
    return foundFactors;
}

inline bool primes::isPrime(const unsigned long long n) {
    return isPrime(n, 5);
}

//O(sqrt(n))
//Ω(1)
inline bool primes::isPrime(const unsigned long long n, const unsigned long potentialPrimeFactorFloor) {
    //this switch is slightly faster than any obvious bitwise approaches tested so far, which all need to first check (n < 4)
    switch (n) {
        case 0ull: 
        case 1ull: return false;
        case 2ull: 
        case 3ull: return true;
    }

    //handles these two cases separately to allow optimizations in the loop below
    if (n % 2ull == 0ull || n % 3ull == 0ull) return false;

    //greatest integer <= the sqrt of n, truncation ok
    const unsigned long maxLessorDivisor { (unsigned long)sqrt(n) };

    //iterates through all odd numbers from at least 5 through the greatest odd int <= n's sqrt inclusive
    for (unsigned long i { std::max(potentialPrimeFactorFloor, 5ul) }; i <= maxLessorDivisor; i += 2ul) {
        //skip multiples of 3 (very common case); severely diminishing returns for numbers beyond this
        //additionally checking for 5 heuristically appears to slow runtime on average by ~1.25x, whereas the case with no such skips at all is ~1.5x
        if (i % 3ul == 0) i += 2ul;
        //if n is divisible by any of these values, n is not prime
        if (n % i == 0) return false;
    }
    return true;
}
