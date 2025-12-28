//
// Created by abdellah on 12/27/2025.
//

#include "../../include/state/GameOverState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <fstream>

#include <game/Game.h>
#include <state/LevelState.h>
#include <utils/ResourceManager.h>

namespace representation {

// =========== GameOverState ctor [START] ===========
GameOverState::GameOverState(StateManager& manager, Game& game, int level, int score)
    : State(manager, game), _level(std::max(1, level)), _final_score(std::max(0, score)) {
    buildUi();
}
// =========== GameOverState ctor [END] ===========

// =========== Center text helper [START] ===========
static void centerText(sf::Text& t, const float x, const float y) {
    const sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(x, y);
}
// =========== Center text helper [END] ===========

void GameOverState::buildUi() {
    const auto win = _game.getWindow().getSize();

    _overlay.setSize({static_cast<float>(win.x), static_cast<float>(win.y)});
    _overlay.setFillColor(sf::Color(0, 0, 0, 200));

    auto& rm = ResourceManager::instance();
    const auto& font = rm.getFont(Font::Crackman);

    const float cx = win.x * 0.5f;

    // =========== Title [START] ===========
    _title.setFont(font);
    _title.setCharacterSize(static_cast<unsigned>(win.y * 0.12f));
    _title.setFillColor(sf::Color::White);
    _title.setString("GAME OVER");
    centerText(_title, cx, win.y * 0.20f);
    // =========== Title [END] ===========

    // =========== Level + score [START] ===========
    _level_text.setFont(font);
    _level_text.setCharacterSize(static_cast<unsigned>(win.y * 0.06f));
    _level_text.setFillColor(sf::Color::White);
    _level_text.setString("Level: " + std::to_string(_level));
    centerText(_level_text, cx, win.y * 0.35f);

    _score_text.setFont(font);
    _score_text.setCharacterSize(static_cast<unsigned>(win.y * 0.06f));
    _score_text.setFillColor(sf::Color::White);
    _score_text.setString("Score: " + std::to_string(_final_score));
    centerText(_score_text, cx, win.y * 0.43f);
    // =========== Level + score [END] ===========

    // =========== High score line (if any) [START] ===========
    if (!_high_score_text.getString().isEmpty()) {
        _high_score_text.setFont(font);
        _high_score_text.setCharacterSize(static_cast<unsigned>(win.y * 0.055f));
        _high_score_text.setFillColor(sf::Color::White);
        centerText(_high_score_text, cx, win.y * 0.51f);
    }
    // =========== High score line (if any) [END] ===========

    // =========== Options [START] ===========
    auto makeOption = [&](const std::string& s, float y) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(static_cast<unsigned>(win.y * 0.065f));
        t.setFillColor(sf::Color::White);
        t.setString(s);
        centerText(t, cx, y);
        return t;
    };

    _options.clear();
    _options.push_back(makeOption("Retry", win.y * 0.62f));
    _options.push_back(makeOption("Menu", win.y * 0.74f));

    _selected = 0;
    refreshSelection();
    // =========== Options [END] ===========
}

void GameOverState::refreshSelection() {
    for (int i = 0; i < static_cast<int>(_options.size()); ++i)
        _options[i].setFillColor(i == _selected ? sf::Color::Yellow : sf::Color::White);
}

void GameOverState::onResize(unsigned width, unsigned height) { buildUi(); }

void GameOverState::activate() const {
    if (_selected == 0) {
        // Pop GameOverState and old LevelState and push a new one to start a level
        _state_manager.popState();
        _state_manager.popState();
        _state_manager.pushState(std::make_unique<LevelState>(_state_manager, _game, 1, 0, 3));
        return;
    }

    _state_manager.popToMenu();
}

// =========== Input [START] ===========
void GameOverState::handleInput() {
    sf::Event e{};
    auto& window = _game.getWindow();

    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();

        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::W || e.key.code == sf::Keyboard::Up) {
                _selected = (_selected - 1 + static_cast<int>(_options.size())) % static_cast<int>(_options.size());
                refreshSelection();
            } else if (e.key.code == sf::Keyboard::S || e.key.code == sf::Keyboard::Down) {
                _selected = (_selected + 1) % static_cast<int>(_options.size());
                refreshSelection();
            } else if (e.key.code == sf::Keyboard::Enter) {
                activate();
                return;
            } else if (e.type == sf::Event::Resized) {
                window.setView(sf::View(
                    sf::FloatRect(0.f, 0.f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));

                onResize(e.size.width, e.size.height);
            }
        }
    }
}
// =========== Input [END] ===========


// =========== Render [START] ===========
void GameOverState::render(sf::RenderWindow& window) {
    window.draw(_overlay);
    window.draw(_title);
    window.draw(_level_text);
    window.draw(_score_text);
    if (!_high_score_text.getString().isEmpty())
        window.draw(_high_score_text);

    for (const auto& t : _options)
        window.draw(t);
}
// =========== Render [END] ===========

} // namespace representation
