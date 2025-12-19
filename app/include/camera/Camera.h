#ifndef PACMANPROJECT_CAMERA_H
#define PACMANPROJECT_CAMERA_H

#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "utils/ResourceManager.h"

namespace representation {

    /** @class Camera
     * I made this class a Singleton because I need only one instance of it.
     ***/
    class Camera {
        unsigned _window_width{0};
        unsigned _window_height{0};

        float world_models_width{0.f};
        float world_models_height{0.f};

        float game_play_start_y;
        float game_play_end_y;

        // Private constructor → singleton
        Camera() = default;

    public:
        // Disable copy & move
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;

        // Singleton access
        static Camera& instance() {
            static Camera instance;
            return instance;
        }

        // Explicit initialization
        void init(unsigned windowWidth, unsigned windowHeight) {
            _window_width = windowWidth;
            _window_height = windowHeight;

            world_models_width  = 2.0f / static_cast<float>(ResourceManager::getMap().front().size());
            world_models_height = 2.0f / static_cast<float>(ResourceManager::getMap().size());

            game_play_start_y = _window_height * .1;
            game_play_end_y = _window_height * .9;
        }

        [[nodiscard]] sf::Vector2f worldToScreen(float x, float y) const;

        [[nodiscard]] float getTileWidth() const;
        [[nodiscard]] float getTileHeight() const;
    };

} // namespace representation

#endif // PACMANPROJECT_CAMERA_H
