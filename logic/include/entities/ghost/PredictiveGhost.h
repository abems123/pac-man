//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_PREDICTIVEGHOST_H
#define PACMANPROJECT_PREDICTIVEGHOST_H
#include "Ghost.h"

namespace logic {
    class PredictiveGhost : public Ghost {
    public:
        PredictiveGhost(float x, float y) : Ghost(x, y) {
            _ghostType = GhostType::Predictive;
        }

        void computeTarget(World *world, PacMan *pac_man) override;

        void stepTowardTarget(World *world) override;

        void decideDirection() override;
    };
}

#endif //PACMANPROJECT_PREDICTIVEGHOST_H
