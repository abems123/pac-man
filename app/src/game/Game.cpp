//
// Created by abdellah on 12/12/25.
//

#include "game/Game.h"

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <utility>

#include "state/MenuState.h"

namespace representation {
void Game::checkMapConsistent() {
    // Check if the map is consistent, it must be of ratio 16:7.2, throw an error if it's not
    auto map = ResourceManager::getMap();

    int _first_line_len = map.front().size();

    const bool inconsistent_lines_lengths =
        std::ranges::any_of(map, [_first_line_len](const auto& line) { return line.size() != _first_line_len; });

    if (const bool _correct_ratio = 2.1 <= static_cast<float>(_first_line_len) / map.size() &&
                                    static_cast<float>(_first_line_len) / map.size() <= 2.3;
        inconsistent_lines_lengths || !_correct_ratio) {
        throw std::runtime_error(
            "Invalid map: map is not 16:7.2, or some row has different number of elements than other rows");
    }
}

Game::Game() : _window(sf::VideoMode::getDesktopMode(), "Pac-Man") {
    checkMapConsistent();

    _window.setFramerateLimit(60);

    // initial menu
    _stateManager.pushState(std::make_unique<MenuState>(_stateManager, *this));

    // ======== Camera Singleton initializing [START] ========
    Camera::instance().init(_window.getSize().x, _window.getSize().y);
    _windowSize = _window.getSize();
    // ======== Camera Singleton initializing [END] ========
}

void Game::run() {
    while (_window.isOpen()) {
        logic::Stopwatch::getInstance().tick();

        // Let the active state process input
        _stateManager.handleInput();

        // Let the active state update logic
        _stateManager.update(logic::Stopwatch::getInstance().dt());

        // Draw everything
        _window.clear(sf::Color::Black);
        _stateManager.render(_window);
        _window.display();
    }
}
} // namespace representation
