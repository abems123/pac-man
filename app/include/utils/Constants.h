//
// Created by abdellah on 12/17/25.
//

#pragma once

#include <cmath>

namespace representation {
struct Constants {
    /** Left padding in the sprite sheet before the first sprite (in pixels). */
    static constexpr int START_LEFT_SPACE = 1;

    /** Top padding in the sprite sheet before the first sprite (in pixels). */
    static constexpr int START_TOP_SPACE = 4;

    /** Pixel gap between sprites in the sprite sheet. */
    static constexpr int SPACE_BETWEEN_SPRITES = 15;

    /** Size of one sprite tile in the sheet (width = height, in pixels). */
    static constexpr int SPRITE_SIZE = 35;

    /** Small tolerance for float comparisons. */
    static constexpr float EPS = 1e-4f;

    /** True if x and y are close enough (within EPS). */
    static bool compareFloats(float x, float y) { return std::fabs(x - y) < EPS; }
};
} // namespace representation
