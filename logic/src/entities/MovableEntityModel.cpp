//
// Created by abdellah on 12/13/25.
//

#include "../../include/entities/MovableEntityModel.h"

#include <utils/Stopwatch.h>

namespace logic {
MovableEntityModel::MovableEntityModel(const MovableEntityModel& that) : EntityModel(that) {}

MovableEntityModel& MovableEntityModel::operator=(const MovableEntityModel& that) {
    if (this != &that) {
        _direction = that._direction;
        _speed = that._speed;
        _x = that._x;
        _y = that._y;
    }
    return *this;
}

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

void MovableEntityModel::update(const float dt) {
    move(dt);
}
void MovableEntityModel::move(const float dt) {
    const float dist = _speed * dt;

    float dx = (_direction == Direction::Left ? -1.f : _direction == Direction::Right ? 1.f : 0.f) * dist;

    float dy = (_direction == Direction::Up ? -1.f : _direction == Direction::Down ? 1.f : 0.f) * dist;

    float nextX = getPosition().first + dx;
    float nextY = getPosition().second + dy;

    setPosition(nextX, nextY);
    notify(EventType::Moved);
}
} // namespace logic
