//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_GHOST_Model
#define PACMANPROJECT_GHOST_Model
#include "entities/EntityModel.h"
#include "entities/MovableEntityModel.h"

namespace logic {
class PacMan;
class World;

class Ghost : public MovableEntityModel {
protected:
    double _target_x, _target_y;
    double _spawn_x, _spawn_y;
    GhostMode _mode = GhostMode::Center;
    float frightenedTimer = 0.f;

    std::vector<Direction> available_directions;

    static int manhattan(int r1, int c1, int r2, int c2);
    static int dr(Direction d);
    static int dc(Direction d);
    static Direction opposite(Direction d);

    virtual void computeTarget() = 0;

    virtual void stepTowardTarget() = 0;


    virtual void decideDirection() = 0;

public:
    using MovableEntityModel::MovableEntityModel;

    Ghost(const float x, const float y, const EntityType type) : MovableEntityModel(x, y, type) { _speed = 0.2f; }

    void enterFrightened(float duration);

    [[nodiscard]] bool isFrightened() const { return frightenedTimer > 0.f; }

    void setMode(const GhostMode mode) { _mode = mode; };

    /** @brief Returns the current behavior mode of the ghost.
     *
     * Used by the world logic to apply special rules (e.g. allow entering the
     * ghost house only when the ghost is Dead).
     *
     * @return Current GhostMode.
     */
    GhostMode getMode() const { return _mode; }

    void addAvailableDir(const Direction dir) { available_directions.push_back(dir); }
    void clearDirections() { available_directions.clear(); }

    void update(World* world, float dt);
};
} // namespace logic

#endif // PACMANPROJECT_GHOST_Model
