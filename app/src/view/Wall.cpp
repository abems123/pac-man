//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Wall.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "camera/Camera.h"
#include "utils/AnimationManager.h"
#include "utils/Constants.h"

namespace representation {
    Wall::Wall(const std::shared_ptr<logic::EntityModel> &model) : EntityView(model, 0, 8, 2) {
        _shape.setFillColor(sf::Color::Blue);
        _shape.setSize({Camera::instance().getTileWidth(), Camera::instance().getTileHeight()});

        auto [x, y] = _model.lock()->getPosition();
        _shape.setPosition(Camera::instance().worldToScreen(x, y));

    }

    void Wall::onNotify(const logic::EventType &event) {
    }

    void Wall::update(float dt) {
        EntityView::update(dt);
    }

    void Wall::render(sf::RenderWindow &window) const {
        // EntityView::render(window);

        window.draw(_shape);
    }
}
