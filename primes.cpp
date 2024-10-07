#include "primes.hpp"

Factorization primes::primeFactorization(uint64_t n) {
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
    //divides n by all odd primes until reaching the value of each of n's prime factors,
    //possibly excluding the greatest prime factor iff the square of said factor does not divide n
    uint64_t divisor { 1 };
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

inline bool primes::isPrime(const uint64_t n) {
    return isPrime(n, 5);
}

inline bool primes::isPrime(const uint64_t n, const uint64_t potentialFactorFloor) {
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
    const uint32_t maxLessorDivisor { (uint32_t)sqrt(n) };

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

// bool primes::bailliePSW(const uint64_t n) {
//     if (n < 0x10000) return isPrime(n);

//     uint64_t d = n - 1;
//     uint32_t s = 0;
//     while (!(d & 0b1)) { 
//         d >>= 1;  
//         ++s;
//     }
//     bool sprpPassed = false;
//     for (uint32_t r = 0; r < s; ++r) {
//         if (1u >> ((1u >> r) * d) % n == 1u) {
//             sprpPassed = true;
//             break;
//         }
//     }
//     if (!sprpPassed) return false;

//     int jacobiD = 5;
//     while (jacobiSymbol(jacobiD, n) != -1) {
//         jacobiD = -jacobiD;
//         jacobiD += (jacobiD > 0 ? 2 : -2);
//     }

//     const int q = (1 - jacobiD) / 4;
//     if (lucasSequence(d, 1, q) % n == 0) {
//         for (uint32_t r = 0; r < s; ++r) 
//             if ((unsigned int)lucasSequence((1u >> r) * d, 1, q) == n - 1) return true;
//     }
//     return false;
// }

// int64_t primes::lucasSequence(const int64_t d, const int p, const int q) {
//     std::array<int64_t, 3> values = { 2, p };
//     for (int64_t i = 2; i < d; ++i) {
//         values[i % 3] = p * values[(i - 1) % 3] + q * values[(i - 2) % 3];
//     }
//     return values[d % 3];
// }

// int primes::jacobiSymbol(int64_t a, int64_t n) {
//     int result = 1;
//     int64_t r;
//     a = a % n;
//     while (a != 0) {
//         while (a % 2 == 0) {
//             a /= 2;
//             r = n % 8;
//             if (r == 3 || r == 5) result = -result;
//         }
//         r = n;
//         n = a;
//         if (r % 4 == 3 && a % 4 == 3) result = -result;
//         a = r % n;
//     }
//     if (n == 1) return result;
//     else return 0;
// }
