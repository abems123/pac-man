//
// Created by abdellah on 12/17/25.
//

#ifndef PACMANPROJECT_CONSTANTS_H
#define PACMANPROJECT_CONSTANTS_H
#include <cmath>

namespace representation {
struct Constants {
    static constexpr int START_LEFT_SPACE = 1;
    static constexpr int START_TOP_SPACE = 4;
    static constexpr int SPACE_BETWEEN_SPRITES = 15;
    static constexpr int SPRITE_SIZE = 35;
    static constexpr float EPS = 1e-4f;
    static constexpr float COIN_RADIUS = 5.f;

    static bool compareFloats(float x, float y) { return std::fabs(x - y) < EPS; }
};
} // namespace representation
#endif // PACMANPROJECT_CONSTANTS_H
