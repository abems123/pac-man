//
// Created by abdellah on 12/13/25.
//

#include "../../include/entities/MovableEntityModel.h"

#include <cmath>
#include <events/EventType.h>
#include <utils/Stopwatch.h>

namespace logic {
MovableEntityModel::MovableEntityModel(const float x, const float y, const EntityType type, const float speed)
    : EntityModel(x, y, type), _speed(speed) {}

void MovableEntityModel::setDirection(const Direction direction) {
    if (this->_direction == direction)
        return;

    this->_direction = direction;

    switch (direction) {
    case Direction::Left:
        notify(EventType::DirectionChangedLeft);
        break;
    case Direction::Up:
        notify(EventType::DirectionChangedUp);
        break;
    case Direction::Down:
        notify(EventType::DirectionChangedDown);
        break;
    case Direction::Right:
        notify(EventType::DirectionChangedRight);
        break;
    }
}
void MovableEntityModel::setMove(const Direction dir, const bool can_move) {
    if (dir == Direction::Right) {
        _can_move_right = can_move;
    } else if (dir == Direction::Left) {
        _can_move_left = can_move;
    } else if (dir == Direction::Up) {
        _can_move_up = can_move;
    } else if (dir == Direction::Down) {
        _can_move_down = can_move;
    }
}
void MovableEntityModel::update(const float dt) {
    // =========== Move and notify only if we actually moved [START] ===========
    const float oldX = getPosition().first;
    const float oldY = getPosition().second;

    const float dist = _speed * dt;

    float dx = (_direction == Direction::Left ? -1.f : _direction == Direction::Right ? 1.f : 0.f) * dist;

    float dy = (_direction == Direction::Up ? -1.f : _direction == Direction::Down ? 1.f : 0.f) * dist;

    float nextX = getPosition().first + dx;
    float nextY = getPosition().second + dy;

    setPosition(nextX, nextY);

    const float newX = getPosition().first;
    const float newY = getPosition().second;

    constexpr float eps = 1e-6f;
    if (std::fabs(newX - oldX) > eps || std::fabs(newY - oldY) > eps) {
        notify(EventType::Moved);
    }
    // =========== Move and notify only if we actually moved [END] ===========
}

} // namespace logic
