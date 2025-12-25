//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/LevelState.h"

#include <SFML/Window/Event.hpp>

#include "game/Game.h"
#include "score/Score.h"
#include "state/PausedState.h"
#include "view/EntityView.h"

namespace representation {
LevelState::LevelState(StateManager& manager, Game& game, int level)
    : State(manager, game), _factory(std::make_shared<ConcreteFactory>(&_views)) {
    _world = std::make_unique<logic::World>(_factory);

    score.setFont(ResourceManager::instance().getFont(Font::Main_Font));
    score.setCharacterSize(game.getWindow().getSize().y * .05);
    score.setPosition(game.getWindow().getSize().x * .09, 0);
    score.setColor(sf::Color::White);

    _pac_loc.setFont(ResourceManager::instance().getFont(Font::Main_Font));
    _pac_loc.setCharacterSize(game.getWindow().getSize().y * .05);
    _pac_loc.setPosition(game.getWindow().getSize().x * .2, 0);
    _pac_loc.setColor(sf::Color::White);
}

void LevelState::handleInput() {
    using sf::Keyboard;

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        _world->changePacmanDirection(logic::Direction::Right);
    } else if (Keyboard::isKeyPressed(Keyboard::A))
        _world->changePacmanDirection(logic::Direction::Left);
    else if (Keyboard::isKeyPressed(Keyboard::W))
        _world->changePacmanDirection(logic::Direction::Up);
    else if (Keyboard::isKeyPressed(Keyboard::S))
        _world->changePacmanDirection(logic::Direction::Down);
    else if (Keyboard::isKeyPressed(Keyboard::Escape))
        stateManager.pushState(std::make_unique<PausedState>(stateManager, game));

    sf::Event event{};
    auto& window = game.getWindow();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::Resized) {
            // TODO: make the window dynamic, the game must always be beautiful on screen
            // TODO: so when the player resizes the window, everything should be moved
            // TODO: and resized, if height>width, extras are shown on top and bottom
            // TODO: else if width>height, extras are shown on right and left
            // TODO: else resize the window to make space
            // Camera::instance().init(event.size.width, event.size.height);
            // for (auto v : _views) {
            // v->updatePosition();
            // }
        }
    }
}

void LevelState::update(const float dt) {
    for (const auto& v : _views) {
        if (v) {
            v->update(dt);
            if (v->modelExpired())
                std::erase(_views, v);
        }
    }

    _world->update(dt);

    _pac_loc.setString("X: " + std::to_string(_world->getPacMan()->getPosition().first) +
                       ", y: " + std::to_string(_world->getPacMan()->getPosition().second));
    score.setString(std::to_string(_world->getScore()->getScore()));
}

void LevelState::render(sf::RenderWindow& window) {
    for (const auto& view : _views) {
        view->render(window);
    }
    window.draw(score);
    window.draw(_pac_loc);
}
} // namespace representation
