//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

namespace logic {
    void EntityModel::update(float dt) {
    }

    void EntityModel::move(float dx, float dy) {
    }

    void EntityModel::setPosition(float x, float y) {
    }

    Position EntityModel::getPosition() const {
        return position;
    }

    Direction EntityModel::getDirection() const {
        return direction;
    }

    void EntityModel::setDirection(Direction direction) {
        this->direction = direction;
    }
}
