//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_GHOST_H
#define PACMANPROJECT_GHOST_H
#include "entities/EntityModel.h"


class PacManModel;
class World;

namespace logic {
    class Ghost : public EntityModel {
        enum class GhostState { Chase, Scatter, Fear, Dead };

        Position target = {};

        Position spawn = {};

        GhostState state = GhostState::Chase;

        float frightenedTimer = 0.f;

    public:
        virtual void computeTarget(World *world, PacManModel *pac_man) = 0;

        void enterFrightened(float duration);

        [[nodiscard]] bool isFrightened() const { return frightenedTimer > 0.f; };

        virtual void stepTowardTarget(World *world);
    };
}



#endif //PACMANPROJECT_GHOST_H
