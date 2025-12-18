//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

namespace logic {
    EntityModel::EntityModel(const EntityModel &that) : Subject(that) {
        _x = that._x;
        _y = that._y;
    }

    void EntityModel::update(float dt) {
    }


    void EntityModel::setPosition(float x, float y) {
        _x = x;
        _y = y;
    }

    std::pair<float, float> EntityModel::getPosition() const {
        return {_x, _y};
    }
}
