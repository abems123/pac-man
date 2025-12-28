//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "Ghost.h"
#include "LookaheadChaseGhost.h"

namespace logic {
class DirectChaseGhost final : public LookaheadChaseGhost {
public:
    DirectChaseGhost(float x, float y);
};
} // namespace logic
