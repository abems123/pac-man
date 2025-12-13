//
// Created by abdellah on 12/12/25.
//


#include "game/Game.h"

#include <SFML/Window/Event.hpp>

#include "state/MenuState.h"

namespace representation {
    Game::Game()
        : window(sf::VideoMode::getDesktopMode(), "Pac-Man"),
          camera(window.getSize().x, window.getSize().y),
          factory(camera) {
        window.setFramerateLimit(60);

        // initial menu
        stateManager.pushState(std::make_unique<MenuState>(stateManager, *this));
    }

    void Game::run() {
        sf::Clock clock;
        while (window.isOpen()) {
            // 1. Calculate delta time
            float dt = clock.restart().asSeconds();

            // 2. Let the active state process input
            stateManager.handleInput();

            // 3. Let the active state update logic
            stateManager.update(dt);

            // 4. Draw everything
            window.clear(sf::Color::Black);
            stateManager.render(window);
            window.display();
        }
    }

}
