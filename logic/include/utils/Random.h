//
// Created by abdellah on 12/11/25.
//

#pragma once

#ifndef PACMANPROJECT_RANDOM_H
#define PACMANPROJECT_RANDOM_H

#include <cstdint>
#include <random>

namespace logic {

/**
 * @brief Singleton random number generator used by the game.
 */
class Random final {
public:
    /**
     * @brief Returns the global Random instance.
     * @return Reference to the singleton instance.
     */
    static Random& instance();

    /** @brief Copy construction is disabled. */
    Random(const Random&) = delete;

    /** @brief Copy assignment is disabled. */
    Random& operator=(const Random&) = delete;

    /** @brief Move construction is disabled. */
    Random(Random&&) = delete;

    /** @brief Move assignment is disabled. */
    Random& operator=(Random&&) = delete;


    /**
     * @brief Generates a uniform integer value in the range [min, max].
     * @param min Inclusive lower bound.
     * @param max Inclusive upper bound.
     * @return Random integer in [min, max].
     */
    int uniformInt(int min, int max);

    /**
     * @brief Returns true with probability p.
     * @param p Probability in range [0, 1].
     * @return True with probability p, otherwise false.
     */
    bool probability(double p);

private:
    /** @brief Constructs the RNG with an initial seed. */
    Random();

    /** @brief Mersenne Twister engine used to generate randomness. */
    std::mt19937 _engine;
};

} // namespace logic

#endif // PACMANPROJECT_RANDOM_H
