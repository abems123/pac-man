//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_PACMANMODEL_H
#define PACMANPROJECT_PACMANMODEL_H
#include "EntityModel.h"
#include "MovableEntityModel.h"

namespace logic {
class PacMan final : public MovableEntityModel {
    bool _can_move_right = false;
    bool _can_move_left = false;
    bool _can_move_down = false;
    bool _can_move_up = false;

public:
    using MovableEntityModel::MovableEntityModel;

    PacMan(float x, float y) : MovableEntityModel(x, y, EntityType::Pacman) { _speed = 0.5f; }

    void setMove(Direction dir, bool can_move);

    void update(float dt) override;
};
} // namespace logic

#endif // PACMANPROJECT_PACMANMODEL_H
