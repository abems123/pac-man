//
// Created by abdellah on 12/12/25.
//

#include "../../include/camera/Camera.h"

#include <iostream>

#include "entities/EntityModel.h"

namespace representation {
    sf::Vector2f Camera::worldToScreen(float x, float y) const {

        float screenX = (x + 1.f) * 0.5f * _window_width;
        float screenY = game_play_start_y + (y + 1.f) * (game_play_end_y - game_play_start_y) * .5f;

        // std::cout << "width: " << width << " height: " << height << std::endl;

        return {screenX, screenY};
    }

    float Camera::getTileWidth() const {
        return (float) _window_width / ResourceManager::getMap().front().size();
    }

    float Camera::getTileHeight() const {
        return (game_play_end_y - game_play_start_y) / ResourceManager::getMap().size();
    }
}
