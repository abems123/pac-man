//
// Created by abdellah on 12/12/25.
//

#include "../../include/camera/Camera.h"

#include <iostream>

#include "entities/EntityModel.h"

namespace representation {
    sf::Vector2f representation::Camera::worldToScreen(const double x, const double y) const {
        float screenX = (x + 1.f) * 0.5f * width;
        float screenY = (y + 1.f) * 0.5f * height;

        // std::cout << "width: " << width << " height: " << height << std::endl;

        return {screenX, screenY};
    }


    float representation::Camera::getTileWidth() const {
        return (width - (width / 10.f) * 2) * world_width;
    }

    float representation::Camera::getTileHeight() const {
        return (height - (height / 10.f) * 2) * world_height;
    }
}
