cmake_minimum_required(VERSION 3.16)
project(PrimeFactorizationCalculator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -flto=auto -O3")
add_executable(primeFactor.exe utils.cpp factorization.cpp primes.cpp rankinglist.cpp timecategories.cpp statset.cpp sequence.cpp main.cpp)