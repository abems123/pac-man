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
    EntityView::EntityView(const std::shared_ptr<logic::EntityModel> &model) : _model(model) {
        // ============= Sprite initialization [START] =============
        _sprite = sf::Sprite(ResourceManager::instance().getTexture());
        _sprite.setScale({
            Camera::instance().getTileWidth() / Constants::SPRITE_SIZE,
            Camera::instance().getTileHeight() / Constants::SPRITE_SIZE
        });
        _sprite.setOrigin(Camera::instance().getTileWidth() / 2.0f, Camera::instance().getTileHeight() / 2.0f);
        // ============= Sprite initialization [END] =============

        // ============= Position Conversion [START] =============
        auto world_position = Camera::instance().worldToScreen(model->getPosition().first,
                                                               model->getPosition().second);
        _position = {world_position.x, world_position.y};
        _sprite.setPosition(_position);
        // ============= Position Conversion [END] =============
    }

    EntityView::EntityView(const std::shared_ptr<logic::EntityModel> &model, int column, int row,
                           int number_of_frames) : EntityView(model) {
        AnimationManager::setFrames(column, row, number_of_frames, _frames);
        _sprite.setTextureRect(_frames.front());
        auto [x,y] = Camera::instance().worldToScreen(model->getPosition().first, model->getPosition().second);
        _sprite.setPosition(x, y);
    }

    void EntityView::update(float dt) {
        if (const auto var = _model.lock()) {
            auto [x, y] = var->getPosition();
            _sprite.setPosition(Camera::instance().worldToScreen(x, y));
        }
    }


    void EntityView::render(sf::RenderWindow &window) const {
        window.draw(_sprite);
    }
}
