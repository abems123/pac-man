//
// Created by abdellah on 12/12/25.
//

#include "view/EntityView.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <utility>

#include "camera/Camera.h"
#include "entities/EntityModel.h"
#include "utils/AnimationManager.h"

#include <utils/Constants.h>

namespace representation {
EntityView::EntityView(const std::shared_ptr<logic::EntityModel>& model) : _model(model) {
    _sprite = sf::Sprite(ResourceManager::instance().getTexture());
    EntityView::updatePosition();
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
        // ============= Sync scale with current tile size [START] =============
        const float sx = Camera::instance().getTileWidth() / Constants::SPRITE_SIZE;
        const float sy = Camera::instance().getTileHeight() / Constants::SPRITE_SIZE;
        const float s = std::min(sx, sy);
        _sprite.setScale({s, s});
        // ============= Sync scale with current tile size [END] =============

        // ============= Position Conversion [START] =============
        const auto world_position =
            Camera::instance().worldToScreen(var->getPosition().first, var->getPosition().second);

        _sprite.setPosition({world_position.x, world_position.y});
        // ============= Position Conversion [END] =============
    }
}

void EntityView::render(sf::RenderWindow& window) const { window.draw(_sprite); }
logic::EntityType EntityView::getModelType() const {
    if (const auto m = _model.lock())
        return m->getType();
    return logic::EntityType::Unknown;
}

} // namespace representation
