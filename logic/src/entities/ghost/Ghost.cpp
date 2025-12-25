//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/Ghost.h"

#include <algorithm>
#include <cmath>

#include <world/World.h>

namespace logic {

bool Ghost::atTileCenter(int row, int col, float eps_x, float eps_y) const {
    if (!_world) return false;

    const auto [center_x, center_y] = _world->getCenter(this);

    const float tile_w = _world->xFromCol(1) - _world->xFromCol(0);
    const float tile_h = _world->yFromRow(1) - _world->yFromRow(0);

    const float cell_left = _world->xFromCol(col);
    const float cell_top  = _world->yFromRow(row);

    const float cell_cx = cell_left + tile_w * 0.5f;
    const float cell_cy = cell_top  + tile_h * 0.5f;

    return std::fabs(center_x - cell_cx) <= eps_x && std::fabs(center_y - cell_cy) <= eps_y;
}

void Ghost::snapToRow(int row) {
    if (!_world) return;
    setPosition(getPosition().first, _world->yFromRow(row));
}

void Ghost::snapToCol(int col) {
    if (!_world) return;
    setPosition(_world->xFromCol(col), getPosition().second);
}

std::pair<int, int> Ghost::ghostCellFromCenterBias() const {
    if (!_world) return {0, 0};

    auto [center_x, center_y] = _world->getCenter(this);

    const float tiny = 1e-4f;
    switch (getDirection()) {
    case Direction::Left:  center_x -= tiny; break;
    case Direction::Right: center_x += tiny; break;
    case Direction::Up:    center_y -= tiny; break;
    case Direction::Down:  center_y += tiny; break;
    default: break;
    }

    const int col = _world->colFromX(center_x);
    const int row = _world->rowFromY(center_y);
    return {row, col};
}

std::pair<int, int> Ghost::pacmanCellFromCenter() const {
    if (!_world) return {0, 0};

    const auto* pm = _world->getPacMan();   // assumes World::getPacMan() exists
    if (!pm) return {0, 0};

    const auto [pm_cx, pm_cy] = _world->getCenter(pm);
    const int pm_col = _world->colFromX(pm_cx);
    const int pm_row = _world->rowFromY(pm_cy);
    return {pm_row, pm_col};
}
int Ghost::manhattan(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

int Ghost::dr(Direction d) {
    return (d == Direction::Up) ? -1 : (d == Direction::Down) ? 1 : 0;
}

int Ghost::dc(Direction d) {
    return (d == Direction::Left) ? -1 : (d == Direction::Right) ? 1 : 0;
}

Direction Ghost::opposite(Direction d) {
    switch (d) {
    case Direction::Left:  return Direction::Right;
    case Direction::Right: return Direction::Left;
    case Direction::Up:    return Direction::Down;
    case Direction::Down:  return Direction::Up;
    default:               return Direction::None;
    }
}

void Ghost::enterFrightened(float duration) {
    frightenedTimer = std::max(frightenedTimer, duration);
}

void Ghost::update(World* world, float dt) {
    if (_world == nullptr) _world = world;

    if (frightenedTimer > 0.f)
        frightenedTimer = std::max(0.f, frightenedTimer - dt);

    decideDirection();
    MovableEntityModel::update(dt);
}

} // namespace logic
