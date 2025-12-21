//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_DIRECTCHASEGHOST_H
#define PACMANPROJECT_DIRECTCHASEGHOST_H
#include "Ghost.h"

namespace logic {
    class DirectChaseGhost : public Ghost {
    public:
        DirectChaseGhost(float x, float y): Ghost(x,y,EntityType::PredictiveGhost) {}

        void computeTarget(World *world, PacMan *pac_man) override;

        void stepTowardTarget(World *world) override;

        void decideDirection() override;

    };
}

#endif //PACMANPROJECT_DIRECTCHASEGHOST_H
