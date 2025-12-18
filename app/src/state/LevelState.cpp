//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/LevelState.h"

#include <iostream>
#include <SFML/Window/Event.hpp>

#include "game/Game.h"
#include "state/PausedState.h"
#include "../../include/view/EntityView.h"

namespace representation {
    LevelState::LevelState(StateManager &manager, Game &game,
                           int level) : State(manager, game),
                                        _factory(std::make_shared<ConcreteFactory>(&_views)) {
        _world = std::make_unique<logic::World>(_factory);
    }

    void LevelState::handleInput() {
        using sf::Keyboard;

        if (Keyboard::isKeyPressed(Keyboard::D)) {
            _world->movePacMan(logic::Direction::Right);
        }
        else if (Keyboard::isKeyPressed(Keyboard::A))
            _world->movePacMan(logic::Direction::Left);
        else if (Keyboard::isKeyPressed(Keyboard::W))
            _world->movePacMan(logic::Direction::Up);
        else if (Keyboard::isKeyPressed(Keyboard::S))
            _world->movePacMan(logic::Direction::Down);
        else if (Keyboard::isKeyPressed(Keyboard::Escape))
            stateManager.pushState(std::make_unique<PausedState>(stateManager, game));

        sf::Event event{};
        auto &window = game.getWindow();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void LevelState::update(const float dt) {
        for (const auto &v: _views) {
            if (v) {
                v->update(dt);
                if (v->modelExpired())
                    std::erase(_views, v);
            }
        }

        _world->update(dt);
    }

    void LevelState::render(sf::RenderWindow &window) {
        for (const auto &view: _views) {
            view->render(window);
        }
    }
}
