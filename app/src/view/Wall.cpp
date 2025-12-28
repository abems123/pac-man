//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Wall.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

#include "camera/Camera.h"
#include "utils/AnimationManager.h"
#include "utils/Constants.h"

#include <entities/Wall.h>

namespace representation {

Wall::Wall(const std::shared_ptr<logic::EntityModel>& model, const bool isGate) : EntityView(model, 0, 8, 2) {
    _is_gate = isGate;

    if (_is_gate) {
        // =========== Gate visual setup [START] ===========

        // Treat the gate as a thin horizontal bar centered in the tile.
        const float tile_w = Camera::instance().getTileWidth();
        const float tile_h = Camera::instance().getTileHeight();
        const float thickness = tile_h / 6.0f;

        _shape = sf::RectangleShape(sf::Vector2f(tile_w - 2.0f, thickness));
        _shape.setFillColor(sf::Color::Blue);

        const auto screenPos =
            Camera::instance().worldToScreen(model->getPosition().first, model->getPosition().second);

        _shape.setPosition(sf::Vector2f(screenPos.x + 1.0f, screenPos.y + (tile_h - thickness) * 0.5f));

        // Avoid running sprite-based wall-type updates for gates.
        updated = true;
        // =========== Gate visual setup [END] ===========
    } else {
        AnimationManager::setFrames(17, 16, 2, _normal_wall_frames);
        AnimationManager::setFrames(17, 12, 4, _edge_wall_frames);
        AnimationManager::setFrames(18, 12, 4, _t_kruispunt_frames);
        AnimationManager::setFrames(16, 12, 4, _tip_walls);

        auto _model_width = 2.0 / ResourceManager::getMap().front().size();
        auto _model_height = 2.0 / ResourceManager::getMap().size();

        bool most_left_wall = Constants::compareFloats(model->getPosition().first, -1.0f);
        bool most_right_wall = Constants::compareFloats(model->getPosition().first, 1.0f - _model_width);

        bool top_left = most_left_wall && Constants::compareFloats(model->getPosition().second, -1.0f);
        bool top_right = Constants::compareFloats(model->getPosition().first, 1.0f - _model_width) &&
                         Constants::compareFloats(model->getPosition().second, -1.0f);
        bool bottom_left = Constants::compareFloats(model->getPosition().first, -1.0f) &&
                           Constants::compareFloats(model->getPosition().second, 1.0f - _model_height);
        bool bottom_right = most_right_wall && Constants::compareFloats(model->getPosition().second, 1.0f - _model_height);

        _sprite.setTextureRect(_normal_wall_frames.front());

        if (top_left) {
            _sprite.setTextureRect(_edge_wall_frames.front());
        } else if (top_right) {
            _sprite.setTextureRect(_edge_wall_frames[1]);
        } else if (bottom_right) {
            _sprite.setTextureRect(_edge_wall_frames[2]);
        } else if (bottom_left) {
            _sprite.setTextureRect(_edge_wall_frames[3]);
        } else if (most_left_wall || most_right_wall)
            _sprite.setTextureRect(_normal_wall_frames.back());

    }
}

void Wall::onNotify(const logic::EventType& event) {}

void Wall::updateWall() {
    if (_is_gate)
        return;

    switch (std::dynamic_pointer_cast<logic::Wall>(_model.lock())->getWallType()) {
    case logic::WallType::Horizontal:
        _sprite.setTextureRect(_normal_wall_frames.front());
        break;
    case logic::WallType::Vertical:
        _sprite.setTextureRect(_normal_wall_frames.back());
        break;
    case logic::WallType::T_S:
        _sprite.setTextureRect(_t_kruispunt_frames[0]);
        break;
    case logic::WallType::T_W:
        _sprite.setTextureRect(_t_kruispunt_frames[1]);
        break;
    case logic::WallType::T_N:
        _sprite.setTextureRect(_t_kruispunt_frames[2]);
        break;
    case logic::WallType::T_E:
        _sprite.setTextureRect(_t_kruispunt_frames[3]);
        break;
    case logic::WallType::Isolated:
        break;
    case logic::WallType::CornerNE:
        _sprite.setTextureRect(_edge_wall_frames[3]);
        break;
    case logic::WallType::CornerNW:
        _sprite.setTextureRect(_edge_wall_frames[2]);
        break;
    case logic::WallType::CornerSE:
        _sprite.setTextureRect(_edge_wall_frames[0]);
        break;
    case logic::WallType::CornerSW:
        _sprite.setTextureRect(_edge_wall_frames[1]);
        break;
    case logic::WallType::Cross:
        break;
    case logic::WallType::EndLeft:
        _sprite.setTextureRect(_tip_walls[3]);
        break;
    case logic::WallType::EndDown:
        _sprite.setTextureRect(_tip_walls[2]);
        break;
    case logic::WallType::EndRight:
        _sprite.setTextureRect(_tip_walls[1]);
        break;
    case logic::WallType::EndUp:
        _sprite.setTextureRect(_tip_walls[0]);
        break;
    }
    updated = true;
}

void Wall::update(const float dt) {
    if (_is_gate) {
        // =========== Gate shape update (resize/camera) [START] ===========
        if (auto m = _model.lock()) {
            const float tile_w = Camera::instance().getTileWidth();
            const float tile_h = Camera::instance().getTileHeight();
            const float thickness = tile_h / 8.0f;

            _shape.setSize(sf::Vector2f(tile_w - 2.0f, thickness));

            const auto screenPos = Camera::instance().worldToScreen(m->getPosition().first, m->getPosition().second);

            _shape.setPosition(sf::Vector2f(screenPos.x + 1.0f, screenPos.y + (tile_h - thickness) * 0.5f));
        }
        // =========== Gate shape update (resize/camera) [END] ===========
        return;
    }

    if (!updated)
        updateWall();
}

void Wall::render(sf::RenderWindow& window) const {
    if (_is_gate) {
        window.draw(_shape);
        return;
    }
    EntityView::render(window);
}
} // namespace representation
