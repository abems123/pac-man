//
// Created by abdellah on 12/12/25.
//

#include "../../include/camera/Camera.h"

#include <iostream>

#include "entities/EntityModel.h"

namespace representation {
    sf::Vector2f Camera::worldToScreen(double x, double y) const {
        float screenX = (x + 1.f) * 0.5f * width;
        float screenY = game_play_start_y + (y + 1.f) * (game_play_end_y - game_play_start_y) * .5f;

        // std::cout << "width: " << width << " height: " << height << std::endl;

        return {screenX, screenY};
    }

    float representation::Camera::getTileWidth() const {
        return (float) width / ResourceManager::instance().getMap().front().size();
    }

    float representation::Camera::getTileHeight() const {
        return (float) (game_play_end_y - game_play_start_y) / ResourceManager::instance().getMap().size();
        //(height - (height / 10.f) * 2) * world_height;
    }
}
