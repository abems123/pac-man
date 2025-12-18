//
// Created by abdellah on 12/13/25.
//

#include "../../include/entities/MovableEntityModel.h"

namespace logic {
    MovableEntityModel::MovableEntityModel(const MovableEntityModel &that) : EntityModel(that) {
        _direction = that._direction;
    }

    void MovableEntityModel::setDirection(const Direction direction) {
        if (this->_direction != direction) {
            this->_direction = direction;
            notify(EventType::DirectionChanged);
        }
    }

    void MovableEntityModel::move(const float dx, const float dy) {
        _x += dx;
        _y += dy;

        notify(EventType::Moved);
    }
}
