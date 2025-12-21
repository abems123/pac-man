//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/PacMan.h"

#include <iostream>

#include "../../logic/include/entities/EntityModel.h"
#include "../../logic/include/entities/PacMan.h"

#include "camera/Camera.h"
#include "events/Event.h"
#include "utils/AnimationManager.h"
#include "utils/Constants.h"


namespace representation {
    void PacMan::update(const float dt) {
        representation::MovableEntityView::update(dt);
    }


    PacMan::PacMan(const std::shared_ptr<logic::PacMan> &model)
        : MovableEntityView(model, {17, 17, 17, 17}, {1, 4, 7, 10}, {2, 2, 2, 2}) {
    }


    void PacMan::onNotify(const logic::EventType &event) {
        MovableEntityView::onNotify(event);
    }

    void PacMan::render(sf::RenderWindow &window) const {
        representation::MovableEntityView::render(window);
    }
}
