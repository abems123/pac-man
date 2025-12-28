//
// Created by abdellah on 12/12/25.
//

#include "../../include/camera/Camera.h"

#include "entities/EntityModel.h"

namespace representation {
void Camera::init(const unsigned windowWidth, const unsigned windowHeight) {
    // =========== Initialize camera and recompute layout [START] ===========
    _window_width = windowWidth;
    _window_height = windowHeight;

    const auto map = ResourceManager::getMap();
    _rows = static_cast<int>(map.size());
    _cols = static_cast<int>(map.front().size());

    updateLayout();
    // =========== Initialize camera and recompute layout [END] ===========
}

void Camera::updateLayout() {
    // =========== Compute gameplay band + fit map inside it [START] ===========
    const float W = static_cast<float>(_window_width);
    const float H = static_cast<float>(_window_height);

    // Fixed HUD bands (top 10%, bottom 10%)
    game_play_start_y = H * _top_hud_ratio;
    game_play_end_y   = H * (1.f - _bottom_hud_ratio);

    const float bandTop = game_play_start_y;
    const float bandH   = std::max(1.f, game_play_end_y - game_play_start_y);

    const float mapAspect = static_cast<float>(_cols) / static_cast<float>(_rows);

    // Fit map into (W x bandH) while preserving aspect ratio
    float mapW = W;
    float mapH = mapW / mapAspect;

    if (mapH > bandH) {
        mapH = bandH;
        mapW = mapH * mapAspect;
    }

    const float left = (W - mapW) * 0.5f;
    const float top  = bandTop + (bandH - mapH) * 0.5f;

    _map_rect_px = {left, top, mapW, mapH};
    // =========== Compute gameplay band + fit map inside it [END] ===========
}

sf::Vector2f Camera::worldToScreen(float x, float y) const {
    // =========== Convert world [-1,1[ to pixels inside map rect [START] ===========
    const float u = (x + 1.f) * 0.5f; // 0..1
    const float v = (y + 1.f) * 0.5f; // 0..1

    const float screenX = _map_rect_px.left + u * _map_rect_px.width;
    const float screenY = _map_rect_px.top  + v * _map_rect_px.height;

    return {screenX, screenY};
    // =========== Convert world [-1,1[ to pixels inside map rect [END] ===========
}

float Camera::getTileWidth() const {
    // Tile width in pixels
    return _map_rect_px.width / static_cast<float>(_cols);
}

float Camera::getTileHeight() const {
    // Tile height in pixels
    return _map_rect_px.height / static_cast<float>(_rows);
}
}
