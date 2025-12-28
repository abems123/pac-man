//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/PacMan.h"
#include <events/EventType.h>

namespace logic {
void PacMan::update(const float dt) {
    if (_can_move_left && _direction == Direction::Left) {
        MovableEntityModel::update(dt);
    } else if (_can_move_up && _direction == Direction::Up) {
        MovableEntityModel::update(dt);
    } else if (_can_move_down && _direction == Direction::Down) {
        MovableEntityModel::update(dt);
    } else if (_can_move_right && _direction == Direction::Right) {
        MovableEntityModel::update(dt);
    }
}
void PacMan::respawn(float x, float y) {
    // =========== Respawn and stay idle until input [START] ===========
    setPosition(x, y);
    setDirection(Direction::None);

    setMove(Direction::Left, false);
    setMove(Direction::Right, false);
    setMove(Direction::Up, false);
    setMove(Direction::Down, false);

    notify(EventType::Respawned);
    // =========== Respawn and stay idle until input [END] ===========
}
} // namespace logic
