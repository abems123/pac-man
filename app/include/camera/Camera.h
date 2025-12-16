//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_CAMERA_H
#define PACMANPROJECT_CAMERA_H
#include <utility>
#include <SFML/System/Vector2.hpp>

#include "utils/ResourceManager.h"


namespace logic {
    struct Position;
}

namespace representation {
    class Camera {
        unsigned width;
        unsigned height;

        float world_width = 2.0/ResourceManager::getMap().front().size();
        float world_height = 2.0/ResourceManager::getMap().size();

    public:
        Camera(const unsigned windowWidth, const unsigned windowHeight, const int rows, const int cols)
            : width(windowWidth), height(windowHeight) {
        }

        [[nodiscard]] sf::Vector2f worldToScreen(double x, double y) const;

        float getTileWidth() const;

        float getTileHeight() const;
    };
}


#endif //PACMANPROJECT_CAMERA_H
