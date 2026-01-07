//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "entities/ghost/Ghost.h"

namespace logic {

/**
 * @brief Ghost that targets a tile ahead of Pac-Man when chasing.
 */
class LookaheadChaseGhost : public Ghost {
public:
    /**
     * @brief Constructs a lookahead-chase ghost at the given world position.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     */
    LookaheadChaseGhost(float x, float y);

    /**
     * @brief Selects the next movement direction at a decision point.
     * @return void
     */
    void decideDirection(const World& world) override;

    /**
     * @brief Computes the current chase target based on Pac-Man's direction and lookahead.
     * @return void
     */
    void computeTarget(const World& world) override;

protected:
    /** @brief Number of tiles to look ahead of Pac-Man when computing the chase target. */
    int LOOKAHEAD = 1;
};

} // namespace logic
