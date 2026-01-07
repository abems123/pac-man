//
// Created by abdellah on 12/11/25.
//

#include "entities/ghost/LookaheadChaseGhost.h"

#include <entities/ghost/DirectChaseGhost.h>

namespace logic {
DirectChaseGhost::DirectChaseGhost(const float x, const float y) : LookaheadChaseGhost(x, y) { LOOKAHEAD = 0; }
} // namespace logic
