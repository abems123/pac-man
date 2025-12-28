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
Wall::Wall(const std::shared_ptr<logic::EntityModel>& model) : EntityView(model, 0, 8, 2) {

    AnimationManager::setFrames(17, 16, 2, _normal_wall_frames);
    AnimationManager::setFrames(17, 12, 4, _edge_wall_frames);
    AnimationManager::setFrames(18, 12, 4, _t_kruispunt_frames);
    AnimationManager::setFrames(16, 12, 4, _tip_walls);

    auto _model_width = 2.0 / ResourceManager::getMap().front().size();
    auto _model_height = 2.0 / ResourceManager::getMap().size();

    // _shape = sf::RectangleShape(sf::Vector2f(Camera::instance().getTileWidth()-2,
    // Camera::instance().getTileHeight()-2)); _shape.setFillColor(sf::Color::Blue);
    // _shape.setPosition(Camera::instance().worldToScreen(model->getPosition().first,model->getPosition().second));

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

void Wall::onNotify(const logic::EventType& event) {}

void Wall::updateWall() {
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
    // std::cout << "Wall updated!!!" << std::endl;
}

void Wall::update(float dt) {
    EntityView::update(dt);

    if (!updated)
        updateWall();
}

void Wall::render(sf::RenderWindow& window) const {
    // window.draw(_shape);
    EntityView::render(window);
}
} // namespace representation
