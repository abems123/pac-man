//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_PREDICTIVEGHOST_H
#define PACMANPROJECT_PREDICTIVEGHOST_H

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
class PredictiveGhost final : public Ghost {
public:
    /**
     * @brief Creates a PredictiveGhost at (x,y).
     */
    PredictiveGhost(float x, float y);

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

private:
    static constexpr int LOOKAHEAD = 1; // tiles "in front of" Pac-Man

    int _target_row = 0;
    int _target_col = 0;

    /**
     * @brief Checks if the ghost is close enough to the center of (row,col) to allow turning.
     */
    bool atTileCenter(int row, int col, float epsX, float epsY) const;

    /**
     * @brief Returns the grid cell one tile ahead of Pac-Man (based on his direction).
     */
    std::pair<int, int> pacmanFrontCell() const;

    /**
     * @brief Snaps perpendicular coordinate to the grid before turning (helps avoid drift).
     */
    void snapPerpendicular(int row, int col, Direction newDir);

    void snapToRow(int row);
    void snapToCol(int col);

    std::pair<int, int> ghostCellFromCenterBias() const;
    std::pair<int, int> pacmanCellFromCenter() const;
};

} // namespace logic

#endif // PACMANPROJECT_PREDICTIVEGHOST_H
