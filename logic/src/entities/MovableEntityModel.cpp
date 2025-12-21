//
// Created by abdellah on 12/13/25.
//

#include "../../include/entities/MovableEntityModel.h"

namespace logic {
    MovableEntityModel::MovableEntityModel(const MovableEntityModel &that) : EntityModel(that) {
    }

    MovableEntityModel &MovableEntityModel::operator=(const MovableEntityModel &that) {
        if (this != &that) {
            _direction = that._direction;
            _speed = that._speed;
            _x = that._x;
            _y = that._y;
        }
        return *this;
    }

    void MovableEntityModel::setDirection(const Direction direction) {
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
        /*
        if (this->_direction != direction) {
            this->_direction = direction;
            notify(EventType::DirectionChanged);
        }*/
    }

    // void MovableEntityModel::move(const float dx, const float dy) {
    //     _x += dx;
    //     _y += dy;
    //
    //     notify(EventType::Moved);
    // }
}
