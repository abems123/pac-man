//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "entities/ghost/Ghost.h"

namespace logic {

/**
 * @brief Ghost that aims for the tile in front of Pac-Man.
 *
 * In chase mode it checks all viable moves and picks the one that minimizes the
 * Manhattan distance to the "front tile" of Pac-Man. Ties are broken randomly.
 *
 * If the ghost is inside the house and not dead, it first heads toward the exit.
 */
class LookaheadChaseGhost : public Ghost {
public:
    /**
     * @brief Creates a LookaheadChaseGhost at (x,y).
     */
    LookaheadChaseGhost(float x, float y);

    /**
     * @brief Chooses a direction when the ghost is at a decision point (near tile center).
     *
     * - Inside house: go toward exit (unless dead).
     * - Chase: minimize Manhattan distance to Pac-Man's front tile.
     * - Fear: maximize distance (optional, if you use Fear for all ghosts).
     */
    void decideDirection() override;

    void computeTarget() override;

    void stepTowardTarget() override;

protected:
    /**
     * @brief Tiles in front of Pac-Man
     */
    int LOOKAHEAD = 1;
};

} // namespace logic
