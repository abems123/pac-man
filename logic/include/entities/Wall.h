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

    class Wall : public EntityModel {
        WallType wall_type = WallType::Isolated;

    public:
        Wall(float x, float y) : EntityModel(x, y, EntityType::Wall) {
        }

        void setWallType(WallType t) { wall_type = t; }
        [[nodiscard]] WallType getWallType() const { return wall_type; }
    };
} // namespace logic

#endif // PACMANPROJECT_WALL_H
