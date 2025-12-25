//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/Ghost.h"

#include <cmath>

namespace logic {

int logic::Ghost::manhattan(int r1, int c1, int r2, int c2) {
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
    case Direction::None:  return Direction::None;
    }
    return Direction::None;
}

void Ghost::update(World* world, float dt) {
    if (!_world && world) _world = world;
    if (!_world) return;

    if (frightenedTimer > 0.f) frightenedTimer = std::max(0.f, frightenedTimer - dt);

    decideDirection();
    MovableEntityModel::update(dt);
}


}
