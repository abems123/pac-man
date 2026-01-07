//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "LookaheadChaseGhost.h"

namespace logic {
class DirectChaseGhost : public LookaheadChaseGhost {
public:
    /**
     * @brief Constructs a direct-chase ghost at the given world position.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     */
    DirectChaseGhost(float x, float y);
};
} // namespace logic
