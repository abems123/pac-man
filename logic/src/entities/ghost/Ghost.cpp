//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/Ghost.h"

#include "utils/Stopwatch.h"


void logic::Ghost::move() {
    const float dt = Stopwatch::getInstance().dt();
    const float dist = _speed * dt;

    float dx = (_direction == Direction::Left ? -1.f : _direction == Direction::Right ? 1.f : 0.f) * dist;

    float dy = (_direction == Direction::Up ? -1.f : _direction == Direction::Down ? 1.f : 0.f) * dist;


    float nextX = getPosition().first + dx;
    float nextY = getPosition().second + dy;

    setPosition(nextX, nextY);
    notify(EventType::Moved);
}

void logic::Ghost::update() {
    decideDirection();
    if (_mode == GhostMode::Chase)
        move();
}
