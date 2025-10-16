#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <chrono>
#include <ctime>
#include <openssl/sha.h>

// Declarations for utility functions. Implementations live in src/utils.cpp
std::string sha256(const std::string& input);
std::string getCurrentTime();

// Template must be defined in header. Return microseconds for better granularity.
template<typename F>
long long measureTime(F func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// Helper to convert microseconds to milliseconds (rounded)
inline long long microsToMillis(long long micros) {
    return (micros + 500) / 1000;
}

#endif