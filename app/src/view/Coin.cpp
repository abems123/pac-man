//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Coin.h"
#include "entities/collectable/Coin.h"

#include "utils/AnimationManager.h"

namespace representation {
Coin::Coin(const std::shared_ptr<logic::Coin>& model) : EntityView(model) {
    _shape = sf::CircleShape(Constants::COIN_RADIUS);
    _shape.setFillColor(sf::Color::White);

    auto [x, y] = _model.lock()->getPosition();
    auto [screen_x, screen_y] = Camera::instance().worldToScreen(x, y);

    _shape.setOrigin(Constants::COIN_RADIUS / 2.0f, Constants::COIN_RADIUS / 2.0f);
    _shape.setPosition(screen_x + Constants::SPRITE_SIZE, screen_y + Constants::SPRITE_SIZE);
}

void Coin::onNotify(const logic::EventType& event) {}

void Coin::render(sf::RenderWindow& window) const {
    // EntityView::render(window);
    window.draw(_shape);
}

void Coin::update(float dt) { EntityView::update(dt); }
} // namespace representation
