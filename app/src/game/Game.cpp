//
// Created by abdellah on 12/12/25.
//


#include "game/Game.h"

#include <SFML/Window/Event.hpp>

#include "state/MenuState.h"

namespace representation {
    // logic::TileMap &Game::getMap() {
    //     return map;
    // }

    void Game::isMapConsistent() {
        // TODO: check if the map is consistent, it must be of ratio 4:5, throw an error if it's not
    }

    Game::Game(): _window(sf::VideoMode::getDesktopMode(), "Pac-Man"), _camera(_window.getSize().x, _window.getSize().y, 0, 0) {

        isMapConsistent();

        _window.setFramerateLimit(60);

        // initial menu
        _stateManager.pushState(std::make_unique<MenuState>(_stateManager, *this));

        _camera = Camera(_window.getSize().x, _window.getSize().y, ResourceManager::getMap().size(), ResourceManager::getMap().front().size());

        _factory = std::make_shared<ConcreteFactory>(_camera);
    }

    void Game::run() {
        while (_window.isOpen()) {
            logic::Stopwatch::getInstance().tick();

            // Let the active state process input
            _stateManager.handleInput();

            // Let the active state update logic
            _stateManager.update(logic::Stopwatch::getInstance().dt());

            // Draw everything
            _window.clear(sf::Color::Cyan);
            _stateManager.render(_window);
            _window.display();
        }
    }
}
