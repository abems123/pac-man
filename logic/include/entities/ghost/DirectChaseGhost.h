//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_DIRECTCHASEGHOST_H
#define PACMANPROJECT_DIRECTCHASEGHOST_H
#include "Ghost.h"

namespace logic {
    class DirectChaseGhost : public Ghost {
    public:
        DirectChaseGhost(float x, float y): Ghost(x,y,EntityType::FollowerGhost) {}

        void computeTarget() override;

        void stepTowardTarget() override;

        void decideDirection() override;


    };
}

#endif //PACMANPROJECT_DIRECTCHASEGHOST_H
