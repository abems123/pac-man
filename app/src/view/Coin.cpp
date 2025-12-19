//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Coin.h"

#include "utils/AnimationManager.h"

namespace representation {
representation::Coin::Coin(const std::shared_ptr<logic::EntityModel>& model) : EntityView(model, 8, 5, 1) {
    _shape = sf::CircleShape(5);
    _shape.setFillColor(sf::Color::White);

    auto [x, y] = _model.lock()->getPosition();
    auto [screen_x, screen_y] = Camera::instance().worldToScreen(x, y);
    _shape.setPosition(screen_x + Constants::SPRITE_SIZE / 2.0, screen_y + Constants::SPRITE_SIZE / 2.0);
}

void Coin::onNotify(const logic::EventType& event) {}

void Coin::render(sf::RenderWindow& window) const {
    // EntityView::render(window);
    window.draw(_shape);
}

void Coin::update(float dt) { EntityView::update(dt); }
} // namespace representation
