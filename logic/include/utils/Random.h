//
// Created by abdellah on 12/11/25.
//

#pragma once

#ifndef PACMANPROJECT_RANDOM_H
#define PACMANPROJECT_RANDOM_H

#include <cstdint>
#include <random>

namespace logic {
class Random final {
public:
    // Access the singleton instance
    static Random& instance();

    // Delete copy & move semantics
    Random(const Random&) = delete;

    Random& operator=(const Random&) = delete;

    Random(Random&&) = delete;

    Random& operator=(Random&&) = delete;

    // Uniform real in [min, max)
    double uniformReal(double min, double max);

    // Uniform integer in [min, max]
    int uniformInt(int min, int max);

    // Bernoulli trial (true with probability p)
    bool probability(double p);

    // Reseed (useful for tests or reproducibility)
    void reseed(std::uint32_t seed);

private:
    Random(); // private constructor

    std::mt19937 _engine;
};
} // namespace logic

#endif // PACMANPROJECT_RANDOM_H
