//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_GHOST_Model
#define PACMANPROJECT_GHOST_Model
#include "entities/EntityModel.h"


class World;

namespace logic {
    class PacMan;

    class Ghost : public EntityModel {
        double _target_x, _target_y;

        double _spawn_x, _spawn_y;

        GhostState state = GhostState::Chase;

        float frightenedTimer = 0.f;

    public:
        using EntityModel::EntityModel;

        virtual void computeTarget(World *world, PacMan *pac_man) = 0;

        virtual void stepTowardTarget(World *world) = 0;

        void enterFrightened(float duration);

        [[nodiscard]] bool isFrightened() const { return frightenedTimer > 0.f; }

        ;
    };
}


#endif //PACMANPROJECT_GHOST_Model
