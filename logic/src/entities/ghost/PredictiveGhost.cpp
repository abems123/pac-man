//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/PredictiveGhost.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include <world/World.h>
#include "utils/Random.h"

namespace logic {

PredictiveGhost::PredictiveGhost(float x, float y)
    : Ghost(x, y, EntityType::PredictiveGhost) {
    _is_moving = true;
    // Start moving immediately (setDirection triggers DirectionChanged* events for the view)
    setDirection(Random::instance().probability(0.5) ? Direction::Left : Direction::Right);
}

bool PredictiveGhost::atTileCenter(int row, int col, float epsX, float epsY) const {
    const auto [cx, cy] = _world->getCenter(this);

    const float tileW = _world->xFromCol(1) - _world->xFromCol(0);
    const float tileH = _world->yFromRow(1) - _world->yFromRow(0);

    const float cellLeft = _world->xFromCol(col);
    const float cellTop  = _world->yFromRow(row);

    const float cellCx = cellLeft + tileW * 0.5f;
    const float cellCy = cellTop  + tileH * 0.5f;

    return std::fabs(cx - cellCx) <= epsX && std::fabs(cy - cellCy) <= epsY;
}

void PredictiveGhost::snapToRow(int row) {
    setPosition(getPosition().first, _world->yFromRow(row));
}

void PredictiveGhost::snapToCol(int col) {
    setPosition(_world->xFromCol(col), getPosition().second);
}

std::pair<int,int> PredictiveGhost::ghostCellFromCenterBias() const {
    auto [cx, cy] = _world->getCenter(this);

    // Small bias so we don't flip cells when sitting exactly on a border
    const float tiny = 1e-4f;
    switch (getDirection()) {
    case Direction::Left:  cx -= tiny; break;
    case Direction::Right: cx += tiny; break;
    case Direction::Up:    cy -= tiny; break;
    case Direction::Down:  cy += tiny; break;
    default: break;
    }

    const int col = _world->colFromX(cx);
    const int row = _world->rowFromY(cy);
    return {row, col};
}

std::pair<int,int> PredictiveGhost::pacmanCellFromCenter() const {
    const auto& pm = _world->getPacMan();
    if (!pm) return {0, 0};

    auto [pcx, pcy] = _world->getCenter(pm);
    const int pcol = _world->colFromX(pcx);
    const int prow = _world->rowFromY(pcy);
    return {prow, pcol};
}

void PredictiveGhost::computeTarget() {
    const auto& pm = _world->getPacMan();
    if (!pm) return;

    const auto [prow, pcol] = pacmanCellFromCenter();
    const Direction pd = pm->getDirection();

    // Look LOOKAHEAD tiles in the direction Pac-Man is facing
    int tr = prow + LOOKAHEAD * Ghost::dr(pd);
    int tc = pcol + LOOKAHEAD * Ghost::dc(pd);

    // If that lands in a wall, just aim for Pac-Man's current tile
    if (_world->isWallCell(tr, tc)) {
        tr = prow;
        tc = pcol;
    }

    _target_row = tr;
    _target_col = tc;
}

void PredictiveGhost::stepTowardTarget() {
    // Direction choice happens here; movement itself is handled by MovableEntityModel::move(dt)
    decideDirection();
}

void PredictiveGhost::decideDirection() {
    if (!_world) return;

    // Only chase logic in Chase/Fear modes
    // if (_mode != GhostMode::Chase && _mode != GhostMode::Fear) return;

    // Only decide at (roughly) the center of the current tile
    const auto [row, col] = ghostCellFromCenterBias();

    const float tileW = _world->xFromCol(1) - _world->xFromCol(0);
    const float tileH = _world->yFromRow(1) - _world->yFromRow(0);

    const float epsX = tileW * 0.06f;
    const float epsY = tileH * 0.06f;

    if (!atTileCenter(row, col, epsX, epsY))
        return;

    computeTarget();

    // Check viable moves from this tile (walls filtered out by World)
    const auto viable = _world->getAvailableDirectionsAt(row, col);
    if (viable.empty())
        return;

    int best = std::numeric_limits<int>::max();
    std::vector<Direction> bestDirs;

    for (Direction d : viable) {
        const int nr = row + Ghost::dr(d);
        const int nc = col + Ghost::dc(d);

        const int dist = Ghost::manhattan(nr, nc, _target_row, _target_col);

        if (dist < best) {
            best = dist;
            bestDirs.clear();
            bestDirs.push_back(d);
        } else if (dist == best) {
            bestDirs.push_back(d);
        }
    }

    if (bestDirs.empty())
        return;

    // Random tie-break if multiple directions are equally good
    const Direction chosen = bestDirs[Random::instance().uniformInt(0, (int)bestDirs.size() - 1)];

    // Snap to the grid before turning so it stays aligned like Pac-Man
    if (chosen != getDirection()) {
        if (chosen == Direction::Left || chosen == Direction::Right)
            snapToRow(row);
        else if (chosen == Direction::Up || chosen == Direction::Down)
            snapToCol(col);
    }

    setDirection(chosen);
}

} // namespace logic
