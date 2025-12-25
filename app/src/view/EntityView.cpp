//
// Created by abdellah on 12/12/25.
//

#include "view/EntityView.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <utility>

#include "camera/Camera.h"
#include "entities/EntityModel.h"
#include "utils/AnimationManager.h"

namespace representation {
EntityView::EntityView(const std::shared_ptr<logic::EntityModel>& model) : _model(model) {
    // ============= Sprite initialization [START] =============
    _sprite = sf::Sprite(ResourceManager::instance().getTexture());
    _sprite.setScale({Camera::instance().getTileWidth() / Constants::SPRITE_SIZE,
                      Camera::instance().getTileHeight() / Constants::SPRITE_SIZE});
    // ============= Sprite initialization [END] =============

    updatePosition();
}

EntityView::EntityView(const std::shared_ptr<logic::EntityModel>& model, int column, int row, int number_of_frames)
    : EntityView(model) {
    AnimationManager::setFrames(column, row, number_of_frames, _frames);
    _sprite.setTextureRect(_frames.front());
    auto [x, y] = Camera::instance().worldToScreen(model->getPosition().first, model->getPosition().second);
    _sprite.setPosition(x, y);
}

void EntityView::update(float dt) {}

void EntityView::updatePosition() {
    if (const auto var = _model.lock()) {
        // ============= Position Conversion [START] =============
        auto world_position = Camera::instance().worldToScreen(var->getPosition().first, var->getPosition().second);
        _position = {world_position.x, world_position.y};
        _sprite.setPosition(_position);
        // ============= Position Conversion [END] =============
    }
}

void EntityView::render(sf::RenderWindow& window) const { window.draw(_sprite); }
} // namespace representation
