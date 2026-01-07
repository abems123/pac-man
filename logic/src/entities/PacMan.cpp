//
// Created by abdellah on 12/11/25.
//

#include "../../include/entities/PacMan.h"
#include <events/EventType.h>

namespace logic {
void PacMan::update(World& world, const float dt) {
    MovableEntityModel::update(world, dt);
}

void PacMan::respawn(const float x, const float y) {
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
