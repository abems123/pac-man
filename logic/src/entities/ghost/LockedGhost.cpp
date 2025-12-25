//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/LockedGhost.h"

#include "utils/Random.h"

#include <utils/Stopwatch.h>
#include <world/World.h>

namespace logic {
LockedGhost::LockedGhost(const float x, const float y) : Ghost(x, y, EntityType::LockedGhost) {
    _is_moving = true;

    // start with a random horizontal direction
    setDirection(Random::instance().probability(0.5) ? Direction::Left : Direction::Right);
}

static bool isHorizontal(Direction d) { return d == Direction::Left || d == Direction::Right; }
static bool isVertical(Direction d)   { return d == Direction::Up   || d == Direction::Down; }

bool LockedGhost::isAtTileCenter(int row, int col, float epsX, float epsY) const {
    // work with the entity center to avoid jitter at tile borders
    const auto [cx, cy] = _world->getCenter(this);

    const float halfW = (_world->xFromCol(1) - _world->xFromCol(0)) * 0.5f;
    const float halfH = (_world->yFromRow(1) - _world->yFromRow(0)) * 0.5f;

    const float cellLeft = _world->xFromCol(col);
    const float cellTop  = _world->yFromRow(row);

    const float cellCx = cellLeft + halfW;
    const float cellCy = cellTop  + halfH;

    return std::fabs(cx - cellCx) <= epsX && std::fabs(cy - cellCy) <= epsY;
}

void LockedGhost::decideDirection() {
    if (_world == nullptr) return;

    auto [cx, cy] = _world->getCenter(this);
    const float tiny = 1e-4f;

    // small bias makes tile indexing stable when exactly on an edge
    switch (getDirection()) {
    case Direction::Left:  cx -= tiny; break;
    case Direction::Right: cx += tiny; break;
    case Direction::Up:    cy -= tiny; break;
    case Direction::Down:  cy += tiny; break;
    default: break;
    }

    int col = _world->colFromX(cx);
    int row = _world->rowFromY(cy);

    const float tileW = _world->xFromCol(1) - _world->xFromCol(0);
    const float tileH = _world->yFromRow(1) - _world->yFromRow(0);
    const float step  = Stopwatch::getInstance().dt() * getSpeed();
    const float epsX  = std::max(tileW * 0.05f, step);
    const float epsY  = std::max(tileH * 0.05f, step);


    // only switch directions close to the center of the tile
    if (!isAtTileCenter(row, col, epsX, epsY))
        return;

    const auto viableSet = _world->getAvailableDirectionsAt(row, col);

    auto toVec = [&](const std::set<Direction>& s) {
        return std::vector<Direction>(s.begin(), s.end());
    };

    // prefer not to reverse unless forced
    std::set<Direction> options = viableSet;
    const Direction back = opposite(getDirection());
    options.erase(back);

    auto applyTurn = [&](Direction newDir) {
        // snap onto the grid before turning (keeps it aligned)
        if (isHorizontal(newDir)) {
            setPosition(getPosition().first, _world->yFromRow(row));
        } else {
            setPosition(_world->xFromCol(col), getPosition().second);
        }
        setDirection(newDir);
    };

    // if forward is blocked, pick a new direction
    if (!viableSet.contains(getDirection())) {
        auto v = toVec(options.empty() ? viableSet : options);
        applyTurn(v[Random::instance().uniformInt(0, (int)v.size() - 1)]);
        return;
    }

    // at intersections, sometimes change direction randomly
    if (options.size() > 1 && Random::instance().probability(0.5)) {
        auto v = toVec(options);
        applyTurn(v[Random::instance().uniformInt(0, (int)v.size() - 1)]);
    }
}

LockedGhost::~LockedGhost() = default;

} // namespace logic
