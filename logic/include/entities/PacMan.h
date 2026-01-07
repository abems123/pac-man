//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_PACMANMODEL_H
#define PACMANPROJECT_PACMANMODEL_H
#include "../utils/Constants.h"
#include "EntityModel.h"
#include "MovableEntityModel.h"

namespace logic {
class PacMan final : public MovableEntityModel {

public:
    /** @brief Inherits MovableEntityModel constructors. */
    using MovableEntityModel::MovableEntityModel;

    /**
     * @brief Constructs Pac-Man at the given world position with the default Pac-Man speed.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     */
    PacMan(const float x, const float y) : MovableEntityModel(x, y, EntityType::Pacman, Constants::PACMAN_SPEED) {}

    /**
     * @brief Updates Pac-Man movement and state for the current frame.
     * @param world
     * @param dt Delta time in seconds.
     * @return void
     */
    void update(World& world, float dt) override;

    /**
     * @brief Respawns Pac-Man at a given position and stops all movement until new input.
     * @param x New x position in world coordinates.
     * @param y New y position in world coordinates.
     * @return void
     */
    void respawn(float x, float y);
};
} // namespace logic

#endif // PACMANPROJECT_PACMANMODEL_H
