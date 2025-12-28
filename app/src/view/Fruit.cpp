//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Fruit.h"

#include <utils/AnimationManager.h>
#include <utils/Random.h>

namespace representation {
Fruit::Fruit(const std::shared_ptr<logic::EntityModel>& model) : EntityView(model) {
    AnimationManager::setFrames(12, 0, 4, _frames);
    _sprite.setTextureRect(_frames[logic::Random::instance().uniformInt(0, 3)]);

    auto [x, y] = Camera::instance().worldToScreen(model->getPosition().first, model->getPosition().second);
    _sprite.setPosition(x, y);
}

void Fruit::render(sf::RenderWindow& window) const { EntityView::render(window); }
} // namespace representation
