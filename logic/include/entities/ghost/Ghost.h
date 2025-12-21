//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_GHOST_Model
#define PACMANPROJECT_GHOST_Model
#include "entities/EntityModel.h"
#include "entities/MovableEntityModel.h"


class World;

namespace logic {
    class PacMan;

    class Ghost : public MovableEntityModel {
    protected:
        double _target_x, _target_y;
        double _spawn_x, _spawn_y;
        GhostMode _mode = GhostMode::Center;
        float frightenedTimer = 0.f;

        std::vector<Direction> available_directions;

    public:
        using MovableEntityModel::MovableEntityModel;

        Ghost(float x, float y, EntityType type) : MovableEntityModel(x, y, type) {
        }

        virtual void computeTarget(World *world, PacMan *pac_man) = 0;

        virtual void stepTowardTarget(World *world) = 0;

        void enterFrightened(float duration);

        [[nodiscard]] bool isFrightened() const { return frightenedTimer > 0.f; }

        void setMode(const GhostMode mode) { _mode = mode; };


        void move();

        virtual void decideDirection() = 0;

        void addAvailableDir(const Direction dir) { available_directions.push_back(dir); }
        void clearDirections() { available_directions.clear(); }

        void update();
    };
}


#endif //PACMANPROJECT_GHOST_Model
