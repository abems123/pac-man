#ifndef PACMANPROJECT_CAMERA_H
#define PACMANPROJECT_CAMERA_H

#include "utils/ResourceManager.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace representation {

class Camera {
    /** Current window width in pixels. */
    unsigned _window_width{0};

    /** Current window height in pixels. */
    unsigned _window_height{0};

    /** Amount of the window height reserved for the top HUD area (0..1). */
    float _top_hud_ratio{0.10f};

    /** Amount of the window height reserved for the bottom HUD area (0..1). */
    float _bottom_hud_ratio{0.10f};

    /** Map columns (used for aspect ratio and tile size). */
    int _cols{0};

    /** Map rows (used for aspect ratio and tile size). */
    int _rows{0};

    /** Pixel rectangle where the map must be rendered (keeps aspect ratio). */
    sf::FloatRect _map_rect_px{};

    /** Pixel Y start of gameplay area (below top HUD). */
    float game_play_start_y{0.f};

    /** Pixel Y end of gameplay area (above bottom HUD). */
    float game_play_end_y{0.f};

    Camera() = default;

    /** Recomputes gameplay band and map rectangle for the current window size. */
    void updateLayout();

public:
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

    static Camera& instance() {
        static Camera instance;
        return instance;
    }

    /** Initializes or updates the camera for a new window size.
     *  @param windowWidth Current window width in pixels.
     *  @param windowHeight Current window height in pixels.
     *  @return Nothing.
     */
    void init(unsigned windowWidth, unsigned windowHeight);

    /** Converts world coordinates [-1,1[ to pixel coordinates inside the map rectangle.
     *  @param x World x in [-1,1[.
     *  @param y World y in [-1,1[.
     *  @return Pixel position on screen.
     */
    [[nodiscard]] sf::Vector2f worldToScreen(float x, float y) const;

    /** Returns the tile width in pixels inside the map rectangle.
     *  @return Tile width in pixels.
     */
    [[nodiscard]] float getTileWidth() const;

    /** Returns the tile height in pixels inside the map rectangle.
     *  @return Tile height in pixels.
     */
    [[nodiscard]] float getTileHeight() const;

    /** Returns the pixel rectangle where the map is rendered.
     *  @return Map rectangle in pixels.
     */
    [[nodiscard]] sf::FloatRect getMapRectPx() const { return _map_rect_px; }
};

} // namespace representation

#endif // PACMANPROJECT_CAMERA_H
