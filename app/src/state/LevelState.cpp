//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/LevelState.h"

#include <iostream>
#include <SFML/Window/Event.hpp>

#include "game/Game.h"
#include "state/PausedState.h"

namespace representation {
    void LevelState::handleInput() {
        sf::Event event{};
        auto &window = game.getWindow();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    // Transition to LevelState
                    stateManager.pushState(std::make_unique<PausedState>(stateManager, game));
                    std::cout << "LevelState::handleInput()" << std::endl;
                }
            }
        }
    }

    void LevelState::update(float dt) {
    }

    void LevelState::render(sf::RenderWindow &window) {
    }
}
