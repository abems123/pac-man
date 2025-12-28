//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Coin.h"
#include "entities/collectable/Coin.h"

#include "utils/AnimationManager.h"

namespace representation {
Coin::Coin(const std::shared_ptr<logic::Coin>& model)
    : EntityView(model), _shape(sf::CircleShape()) {
    _shape.setFillColor(sf::Color::White);
    Coin::updatePosition();
}

void Coin::updatePosition() {
    const auto m = _model.lock();
    if (!m)
        return;

    const float tileW = Camera::instance().getTileWidth();
    const float tileH = Camera::instance().getTileHeight();

    const float radius = std::min(tileW, tileH) * 0.08f;

    _shape.setRadius(radius);
    _shape.setOrigin(radius, radius);

    const auto [sx, sy] = Camera::instance().worldToScreen(m->getPosition().first, m->getPosition().second);
    _shape.setPosition(sx + tileW * 0.5f, sy + tileH * 0.5f);
}


void Coin::render(sf::RenderWindow& window) const {
    window.draw(_shape);
}

} // namespace representation
