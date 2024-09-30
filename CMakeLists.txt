cmake_minimum_required(VERSION 3.16)
project(PrimeFactorizationCalculator)

set(CXX_STANDARD_REQUIRED 23)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3")
add_executable(primeFactor.exe utils.cpp primes.cpp stats.cpp sequence.cpp main.cpp)