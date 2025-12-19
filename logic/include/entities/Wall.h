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
    WallType type = WallType::Isolated;
public:
    using EntityModel::EntityModel;

    void setType(WallType t) { type = t; }
    WallType getType() const { return type; }
};
} // namespace logic

#endif // PACMANPROJECT_WALL_H
