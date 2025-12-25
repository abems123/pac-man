//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/PacMan.h"

void logic::PacMan::setMove(const Direction dir, const bool can_move) {
    if (dir == Direction::Right) {
        _can_move_right = can_move;
    } else if (dir == Direction::Left) {
        _can_move_left = can_move;
    } else if (dir == Direction::Up) {
        _can_move_up = can_move;
    } else if (dir == Direction::Down) {
        _can_move_down = can_move;
    }
}
void logic::PacMan::update(float dt) {
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