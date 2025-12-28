//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/VictoryState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <game/Game.h>
#include <state/LevelState.h>

namespace representation {

// =========== Helpers [START] ===========
static void centerText(sf::Text& t, const float x, const float y) {
    const sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(x, y);
}

static unsigned uclamp(const float v, const float lo, const float hi) {
    return static_cast<unsigned>(std::clamp(v, lo, hi));
}
static bool containsPoint(const sf::Text& t, const sf::Vector2f& p) { return t.getGlobalBounds().contains(p); }

static sf::Vector2f mouseWorldPos(const sf::RenderWindow& window) {
    const auto mp = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mp);
}
// =========== Helpers [END] ===========

VictoryState::VictoryState(StateManager& manager, Game& game, const int level, const int score, const int livesLeft)
    : State(manager, game), _level(level), _carry_lives(livesLeft), _final_score(score) {
    buildUi();
}

void VictoryState::buildUi() {
    // =========== Build victory UI (beautiful centered card) [START] ===========
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);
    const float cx = W * 0.5f;

    const auto& rm = ResourceManager::instance();

    const auto& titleFont = rm.getFont(Font::Crackman);
    const auto& uiFont = rm.getFont(Font::PressStart2P);
    const auto& infoFont = rm.getFont(Font::VT323);

    const sf::Color gold(255, 215, 0);
    const sf::Color outlineCol(0, 0, 0, 190);

    const float outline = std::max(2.f, H * 0.004f);

    // Overlay
    _overlay.setSize({W, H});
    _overlay.setFillColor(sf::Color(0, 0, 0, 185));

    // Center card (panel)
    const float panelW = std::clamp(W * 0.58f, 420.f, 920.f);
    const float panelH = std::clamp(H * 0.55f, 280.f, 720.f);
    const float px = cx - panelW * 0.5f;
    const float py = H * 0.22f;

    _panel_shadow.setSize({panelW, panelH});
    _panel_shadow.setPosition(px + W * 0.01f, py + H * 0.012f);
    _panel_shadow.setFillColor(sf::Color(0, 0, 0, 120));

    _panel.setSize({panelW, panelH});
    _panel.setPosition(px, py);
    _panel.setFillColor(sf::Color(15, 15, 20, 235));
    _panel.setOutlineThickness(std::max(2.f, H * 0.0035f));
    _panel.setOutlineColor(sf::Color(255, 255, 255, 80));

    // Title (arcade)
    _title.setFont(titleFont);
    _title.setString("VICTORY!");
    _title.setCharacterSize(uclamp(H * 0.11f, 34.f, 120.f));
    _title.setFillColor(gold);
    _title.setOutlineThickness(outline);
    _title.setOutlineColor(outlineCol);
    centerText(_title, cx, py + panelH * 0.20f);

    // =========== Score + lives (center each line) [START] ===========
    _score_text.setFont(infoFont);
    _score_text.setCharacterSize(uclamp(H * 0.075f, 28.f, 80.f));
    _score_text.setFillColor(sf::Color::White);
    _score_text.setOutlineThickness(std::max(1.f, outline * 0.6f));
    _score_text.setOutlineColor(outlineCol);
    _score_text.setString("Score: " + std::to_string(_final_score));
    centerText(_score_text, cx, py + panelH * 0.42f);

    _lives_text = _score_text; // copy styling
    _lives_text.setString("Lives left: " + std::to_string(_carry_lives));
    centerText(_lives_text, cx, py + panelH * 0.52f);
    // =========== Score + lives (center each line) [END] ===========

    auto makeOption = [&](const std::string& s, const float y) {
        sf::Text t;
        t.setFont(uiFont);
        t.setCharacterSize(uclamp(H * 0.045f, 14.f, 32.f));
        t.setString(s);
        t.setFillColor(sf::Color::White);
        t.setOutlineThickness(std::max(1.f, outline * 0.55f));
        t.setOutlineColor(outlineCol);
        t.setLetterSpacing(1.05f);
        centerText(t, cx, y);
        return t;
    };

    _options.clear();
    _options.push_back(makeOption("Next Level", py + panelH * 0.72f));
    _options.push_back(makeOption("Menu", py + panelH * 0.84f));

    _selected = std::clamp(_selected, 0, static_cast<int>(_options.size()) - 1);
    refreshSelection();
}

void VictoryState::refreshSelection() {
    const sf::Color gold(255, 215, 0);

    for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
        const bool sel = (i == _selected);
        _options[i].setFillColor(sel ? gold : sf::Color::White);
        _options[i].setScale(sel ? 1.06f : 1.f, sel ? 1.06f : 1.f);
    }
}

void VictoryState::onResize(const unsigned, const unsigned) { buildUi(); }

void VictoryState::activate() const {
    if (_selected == 0) {
        _state_manager.popState(); // Victory
        _state_manager.popState(); // Level
        _state_manager.pushState(
            std::make_unique<LevelState>(_state_manager, _game, _level + 1, _final_score, _carry_lives));
        return;
    }

    _state_manager.popToMenu();
}

void VictoryState::handleInput() {
    // =========== Handle victory menu input (keyboard + mouse) [START] ===========
    sf::Event e{};
    auto& window = _game.getWindow();

    while (window.pollEvent(e)) {
        switch (e.type) {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::Resized: {
            window.setView(
                sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));
            onResize(e.size.width, e.size.height);
            break;
        }

        case sf::Event::KeyPressed: {
            if (e.key.code == sf::Keyboard::W || e.key.code == sf::Keyboard::Up) {
                _selected = (_selected - 1 + static_cast<int>(_options.size())) % static_cast<int>(_options.size());
                refreshSelection();
            } else if (e.key.code == sf::Keyboard::S || e.key.code == sf::Keyboard::Down) {
                _selected = (_selected + 1) % static_cast<int>(_options.size());
                refreshSelection();
            } else if (e.key.code == sf::Keyboard::Enter) {
                activate();
                return;
            }
            break;
        }

        case sf::Event::MouseMoved: {
            const sf::Vector2f p = mouseWorldPos(window);

            for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
                if (containsPoint(_options[i], p)) {
                    if (_selected != i) {
                        _selected = i;
                        refreshSelection();
                    }
                    break;
                }
            }
            break;
        }

        case sf::Event::MouseButtonPressed: {
            if (e.mouseButton.button != sf::Mouse::Left)
                break;

            const sf::Vector2f p = mouseWorldPos(window);

            for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
                if (containsPoint(_options[i], p)) {
                    _selected = i;
                    refreshSelection();
                    activate();
                    return;
                }
            }
            break;
        }

        default:
            break;
        }
    }
    // =========== Handle victory menu input (keyboard + mouse) [END] ===========
}

void VictoryState::update(float) {}

void VictoryState::render(sf::RenderWindow& window) {
    window.draw(_overlay);
    window.draw(_panel_shadow);
    window.draw(_panel);
    window.draw(_title);
    window.draw(_score_text);
    window.draw(_lives_text);
    for (const auto& t : _options)
        window.draw(t);
}

} // namespace representation