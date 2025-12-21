//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Ghost.h"

#include "entities/ghost/LockedGhost.h"
#include "utils/AnimationManager.h"

namespace representation {
    Ghost::Ghost(const std::shared_ptr<logic::Ghost> &model, int ghost_col) : MovableEntityView(
        model, {ghost_col, ghost_col, ghost_col, ghost_col}, {0, 2, 4, 6}, {2, 2, 2, 2}) {
        is_moving = true;
    }

    void Ghost::onNotify(const logic::EventType &event) {
        MovableEntityView::onNotify(event);
    }

    void Ghost::update(float dt) {
        MovableEntityView::update(dt);
    }

    void Ghost::render(sf::RenderWindow &window) const {
        MovableEntityView::render(window);
    }
}
