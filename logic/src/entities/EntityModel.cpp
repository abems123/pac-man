//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

namespace logic {
    EntityModel::EntityModel(const EntityModel &that) : Subject(that) {
        bounds = that.bounds;
    }

    void EntityModel::update(float dt) {
    }


    void EntityModel::setPosition(float x, float y) {
    }

    std::pair<double, double> EntityModel::getPosition() const {
        return {bounds.x, bounds.y};
    }

    float EntityModel::getHeight() const {
        return bounds.h;
    }

    float EntityModel::getWidth() const {
        return bounds.w;
    }
}
