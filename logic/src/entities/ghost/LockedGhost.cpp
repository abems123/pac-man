//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/LockedGhost.h"

#include "utils/Random.h"

namespace logic {
    LockedGhost::LockedGhost(const float x, const float y) : Ghost(x, y, EntityType::LockedGhost) {
        if (Random::instance().probability(0.5))
            notify(EventType::DirectionChangedRight);
        else notify(EventType::DirectionChangedLeft);;
    }

    void LockedGhost::computeTarget(World *world, PacMan *pac_man) {
    }

    void LockedGhost::stepTowardTarget(World *world) {
    }

    LockedGhost::~LockedGhost() = default;

    void LockedGhost::decideDirection() {
        setDirection(available_directions[Random::instance().uniformInt(0, available_directions.size())]);
    }
}
