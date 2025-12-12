//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

void EntityModel::update(float dt) {

}

void EntityModel::move(const int dx, const int dy) {
    position.x += dx;
    position.y += dy;
}

void EntityModel::setPosition(const int x, const int y) {
    position.x = x;
    position.y = y;
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
