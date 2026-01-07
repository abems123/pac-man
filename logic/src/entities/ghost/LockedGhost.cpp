//
// Created by abdellah on 12/11/25.
//

#include "entities/ghost/LockedGhost.h"

#include "utils/Random.h"

#include <utils/Stopwatch.h>
#include <world/World.h>

namespace logic {
LockedGhost::LockedGhost(const float x, const float y) : Ghost(x, y, EntityType::LockedGhost) {
    // start with a random horizontal direction
    setDirection(Random::instance().probability(0.5) ? Direction::Left : Direction::Right);
}

static bool isHorizontal(Direction d) { return d == Direction::Left || d == Direction::Right; }

void LockedGhost::decideDirection(const World& world) {
    if (_mode != GhostMode::Chase && _mode != GhostMode::Fear)
        return;

    auto [cx, cy] = world.getCenter(*this);
    constexpr float tiny = 1e-4f;

    // small bias makes tile indexing stable when exactly on an edge
    switch (getDirection()) {
    case Direction::Left:
        cx -= tiny;
        break;
    case Direction::Right:
        cx += tiny;
        break;
    case Direction::Up:
        cy -= tiny;
        break;
    case Direction::Down:
        cy += tiny;
        break;
    default:
        break;
    }

    const int col = world.colFromX(cx);
    const int row = world.rowFromY(cy);

    const float tileW = world.xFromCol(1) - world.xFromCol(0);
    const float tileH = world.yFromRow(1) - world.yFromRow(0);
    const float step = Stopwatch::getInstance().dt() * getSpeed();
    const float eps_x = std::max(tileW * 0.02f, step);
    const float eps_y = std::max(tileH * 0.02f, step);

    // only switch directions close to the center of the tile
    if (!atTileCenter(row, col, eps_x, eps_y))
        return;

    const auto viableSet = world.getAvailableGhostDirectionsAt(row, col, *this);

    auto toVec = [&](const std::set<Direction>& s) { return std::vector(s.begin(), s.end()); };

    // prefer not to reverse unless forced
    std::set<Direction> options = viableSet;
    const Direction back = opposite(getDirection());
    options.erase(back);

    auto applyTurn = [&](const Direction newDir) {
        // snap onto the grid before turning (keeps it aligned)
        if (isHorizontal(newDir)) {
            setPosition(getPosition().first, world.yFromRow(row));
        } else {
            setPosition(world.xFromCol(col), getPosition().second);
        }
        setDirection(newDir);
    };

    // if forward is blocked, pick a new direction
    if (!viableSet.contains(getDirection())) {
        const auto v = toVec(options.empty() ? viableSet : options);
        applyTurn(v[Random::instance().uniformInt(0, (int)v.size() - 1)]);
        return;
    }

    // at intersections, sometimes change direction randomly
    if (options.size() > 1 && Random::instance().probability(0.5)) {
        const auto v = toVec(options);
        applyTurn(v[Random::instance().uniformInt(0, (int)v.size() - 1)]);
    }
}

LockedGhost::~LockedGhost() = default;

} // namespace logic
