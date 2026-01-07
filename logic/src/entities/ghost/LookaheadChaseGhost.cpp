//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/LookaheadChaseGhost.h"

#include <cmath>
#include <limits>
#include <vector>

#include "utils/Random.h"
#include <world/World.h>

namespace logic {

LookaheadChaseGhost::LookaheadChaseGhost(float x, float y) : Ghost(x, y, EntityType::LookaheadChaseGhost) {}

void LookaheadChaseGhost::computeTarget(const World& world) {
    const auto& pm = world.getPacMan();
    if (!pm)
        return;

    const auto [prow, pcol] = pacmanCellFromCenter();
    const Direction pd = pm->getDirection();

    // Look LOOKAHEAD tiles in the direction Pac-Man is facing
    int tr = prow + LOOKAHEAD * dr(pd);
    int tc = pcol + LOOKAHEAD * dc(pd);

    // If that lands in a wall, just aim for Pac-Man's current tile
    if (world.isWallCell(tr, tc)) {
        tr = prow;
        tc = pcol;
    }

    _target_y = tr;
    _target_x = tc;
}

void LookaheadChaseGhost::decideDirection(const World& world) {
    // Only chase logic in Chase/Fear modes
    if (_mode != GhostMode::Chase && _mode != GhostMode::Fear)
        return;

    // Only decide at (roughly) the center of the current tile
    const auto [cur_row, cur_col] = ghostCellFromCenterBias();

    computeTarget(world);

    // Check viable moves from this tile (walls filtered out by World)
    const auto viable_dirs = world.getAvailableGhostDirectionsAt(cur_row, cur_col, *this);
    if (viable_dirs.empty())
        return;

    int best_dist = std::numeric_limits<int>::max();
    std::vector<Direction> best_dirs;

    for (Direction dir : viable_dirs) {
        const int next_row = cur_row + dr(dir);
        const int next_col = cur_col + dc(dir);

        const int dist_to_target = manhattan(next_row, next_col, _target_y, _target_x);

        if (dist_to_target < best_dist) {
            best_dist = dist_to_target;
            best_dirs.clear();
            best_dirs.push_back(dir);
        } else if (dist_to_target == best_dist) {
            best_dirs.push_back(dir);
        }
    }

    if (best_dirs.empty())
        return;

    // Random tie-break if multiple directions are equally good
    const Direction chosen = best_dirs[Random::instance().uniformInt(0, (int)best_dirs.size() - 1)];

    // Snap to the grid before turning so it stays aligned like Pac-Man
    if (chosen != getDirection()) {
        if (chosen == Direction::Left || chosen == Direction::Right)
            snapToRow(cur_row);
        else if (chosen == Direction::Up || chosen == Direction::Down)
            snapToCol(cur_col);
    }

    setDirection(chosen);
}

} // namespace logic
