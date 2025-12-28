//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WALL_H
#define PACMANPROJECT_WALL_H
#include "EntityModel.h"

namespace logic {
enum class WallType {
    Isolated,
    Horizontal,
    Vertical,
    CornerNE,
    CornerNW,
    CornerSE,
    CornerSW,
    T_N,
    T_E,
    T_S,
    T_W,
    Cross,
    EndLeft,
    EndDown,
    EndRight,
    EndUp
};

class Wall final : public EntityModel {
    /** @brief Visual/structural classification of this wall tile in the maze. */
    WallType wall_type = WallType::Isolated;

public:
    /**
     * @brief Constructs a wall model at the given world position.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     */
    Wall(const float x, const float y) : EntityModel(x, y, EntityType::Wall) {}

    /**
     * @brief Sets the computed wall type for this tile.
     * @param t Wall type to assign.
     * @return void
     */
    void setWallType(const WallType t) { wall_type = t; }

    /**
     * @brief Gets the current wall type for this tile.
     * @return Current wall type.
     */
    [[nodiscard]] WallType getWallType() const { return wall_type; }
};
} // namespace logic

#endif // PACMANPROJECT_WALL_H
