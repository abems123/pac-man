//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

#include "events/Event.h"

namespace logic {
EntityModel::EntityModel(const EntityModel& that) : Subject(that) { _type = that._type; }

EntityModel& EntityModel::operator=(const EntityModel& that) {
    if (this != &that) {
        _x = that._x;
        _y = that._y;
        _type = that._type;
    }
    return *this;
}

void EntityModel::setPosition(float x, float y) {
    _x = x;
    _y = y;
}

} // namespace logic
