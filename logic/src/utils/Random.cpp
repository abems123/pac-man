//
// Created by abdellah on 12/11/25.
//

#include "../../include/utils/Random.h"
#include <chrono>
#include <stdexcept>

namespace logic {

Random::Random() {
    const auto seed = static_cast<std::uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    _engine.seed(seed);
}

Random& Random::instance() {
    static Random instance;
    return instance;
}

double Random::uniformReal(double min, double max) {
    if (min >= max) {
        throw std::invalid_argument("uniformReal: min must be < max");
    }

    std::uniform_real_distribution<double> dist(min, max);
    return dist(_engine);
}

int Random::uniformInt(int min, int max) {
    if (min > max) {
        throw std::invalid_argument("uniformInt: min must be <= max");
    }

    std::uniform_int_distribution<int> dist(min, max);
    return dist(_engine);
}

bool Random::probability(double p) {
    if (p < 0.0 || p > 1.0) {
        throw std::invalid_argument("probability: p must be in [0, 1]");
    }

    std::bernoulli_distribution dist(p);
    return dist(_engine);
}

void Random::reseed(std::uint32_t seed) { _engine.seed(seed); }

} // namespace logic
