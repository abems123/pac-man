//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/EntityModel.h"

#include <cmath>
#include <events/EventType.h>

namespace logic {

void EntityModel::setPosition(float x, float y) {
    // =========== Apply position change without starting movement animation [START] ===========
    constexpr float eps = 1e-6f;

    if (std::fabs(_x - x) <= eps && std::fabs(_y - y) <= eps)
        return;

    _x = x;
    _y = y;

    notify(EventType::PositionChanged);
    // =========== Apply position change without starting movement animation [END] ===========
}

} // namespace logic
