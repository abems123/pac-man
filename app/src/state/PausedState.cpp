//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/PausedState.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <game/Game.h>

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
// =========== Helpers [END] ===========

void PausedState::buildMenu() {
    // =========== Build pause UI (centered card) [START] ===========
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);
    const float cx = W * 0.5f;

    const auto& rm = ResourceManager::instance();

    // Fonts you have in assets/fonts/
    const auto& titleFont = rm.getFont(Font::Crackman);
    const auto& uiFont = rm.getFont(Font::PressStart2P);
    const auto& infoFont = rm.getFont(Font::VT323);

    const sf::Color gold(255, 215, 0);
    const sf::Color outlineCol(0, 0, 0, 190);
    const float outline = std::max(2.f, H * 0.004f);

    // Overlay
    _overlay.setSize({W, H});
    _overlay.setFillColor(sf::Color(0, 0, 0, 185));

    // Panel
    const float panelW = std::clamp(W * 0.56f, 420.f, 920.f);
    const float panelH = std::clamp(H * 0.52f, 260.f, 680.f);
    const float px = cx - panelW * 0.5f;
    const float py = H * 0.24f;

    _panel_shadow.setSize({panelW, panelH});
    _panel_shadow.setPosition(px + W * 0.010f, py + H * 0.012f);
    _panel_shadow.setFillColor(sf::Color(0, 0, 0, 120));

    _panel.setSize({panelW, panelH});
    _panel.setPosition(px, py);
    _panel.setFillColor(sf::Color(14, 14, 18, 235));
    _panel.setOutlineThickness(std::max(2.f, H * 0.0035f));
    _panel.setOutlineColor(sf::Color(255, 255, 255, 70));

    // Title
    _title.setFont(titleFont);
    _title.setString("PAUSED");
    _title.setCharacterSize(uclamp(H * 0.11f, 34.f, 120.f));
    _title.setFillColor(gold);
    _title.setOutlineThickness(outline);
    _title.setOutlineColor(outlineCol);
    centerText(_title, cx, py + panelH * 0.22f);

    // Subtitle (small, readable)
    _subtitle.setFont(infoFont);
    _subtitle.setString("Game is paused");
    _subtitle.setCharacterSize(uclamp(H * 0.055f, 18.f, 52.f));
    _subtitle.setFillColor(sf::Color(255, 255, 255, 200));
    _subtitle.setOutlineThickness(std::max(1.f, outline * 0.5f));
    _subtitle.setOutlineColor(outlineCol);
    centerText(_subtitle, cx, py + panelH * 0.34f);

    // Options
    auto makeOption = [&](const std::string& s, const float y) {
        sf::Text t;
        t.setFont(uiFont);
        t.setString(s);
        t.setCharacterSize(uclamp(H * 0.045f, 14.f, 32.f));
        t.setFillColor(sf::Color::White);
        t.setOutlineThickness(std::max(1.f, outline * 0.55f));
        t.setOutlineColor(outlineCol);
        t.setLetterSpacing(1.05f);
        centerText(t, cx, y);
        return t;
    };

    _options.clear();
    _options.push_back(makeOption("Resume", py + panelH * 0.62f));
    _options.push_back(makeOption("Menu", py + panelH * 0.78f));

    // Hint line
    _hint.setFont(infoFont);
    _hint.setString("Mouse: click  |  Keys: W/S + Enter  |  Esc: resume");
    _hint.setCharacterSize(uclamp(H * 0.040f, 16.f, 40.f));
    _hint.setFillColor(sf::Color(255, 255, 255, 170));
    centerText(_hint, cx, py + panelH * 0.92f);

    // Option “pill” boxes
    _option_boxes.clear();
    _option_boxes.resize(_options.size());
    for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
        const sf::FloatRect b = _options[i].getGlobalBounds();

        sf::RectangleShape box;
        box.setSize({b.width + W * 0.04f, b.height + H * 0.02f});
        box.setOrigin(box.getSize().x * 0.5f, box.getSize().y * 0.5f);
        box.setPosition(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
        box.setFillColor(sf::Color(255, 215, 0, 0)); // hidden until selected
        box.setOutlineThickness(std::max(1.f, H * 0.0025f));
        box.setOutlineColor(sf::Color(255, 215, 0, 0)); // hidden until selected
        _option_boxes[i] = box;
    }

    _selected = std::clamp(_selected, 0, static_cast<int>(_options.size()) - 1);
    updateSelectionVisuals();
    // =========== Build pause UI (centered card) [END] ===========
}

void PausedState::updateSelectionVisuals() {
    // =========== Highlight selected option (gold + pill) [START] ===========
    const sf::Color gold(255, 215, 0);

    for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
        const bool sel = (i == _selected);

        _options[i].setFillColor(sel ? gold : sf::Color::White);
        _options[i].setScale(sel ? 1.06f : 1.f, sel ? 1.06f : 1.f);

        // Show/hide background pill
        _option_boxes[i].setFillColor(sel ? sf::Color(255, 215, 0, 35) : sf::Color(255, 215, 0, 0));
        _option_boxes[i].setOutlineColor(sel ? sf::Color(255, 215, 0, 140) : sf::Color(255, 215, 0, 0));
    }
    // =========== Highlight selected option (gold + pill) [END] ===========
}

void PausedState::render(sf::RenderWindow& window) {
    // =========== Render pause overlay (card) [START] ===========
    window.draw(_overlay);
    window.draw(_panel_shadow);
    window.draw(_panel);

    window.draw(_title);
    window.draw(_subtitle);

    for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
        window.draw(_option_boxes[i]);
        window.draw(_options[i]);
    }

    window.draw(_hint);
    // =========== Render pause overlay (card) [END] ===========
}

PausedState::PausedState(StateManager& manager, Game& game) : State(manager, game) { buildMenu(); }

void PausedState::activateSelected() const {
    // =========== Execute selected action [START] ===========
    if (_selected == 0) {
        _state_manager.popState(); // resume
        return;
    }

    // Exit
    if (_selected == 1) {
        _state_manager.popToMenu();
        return;
    }
    // =========== Execute selected action [END] ===========
}
void PausedState::onResize(const unsigned, const unsigned) {
    // =========== PausedState resize rebuild [START] ===========
    buildMenu();

    // Safety: make sure selection is valid after rebuild
    if (!_options.empty()) {
        _selected = std::clamp(_selected, 0, static_cast<int>(_options.size()) - 1);
        updateSelectionVisuals();
    }
    // =========== PausedState resize rebuild [END] ===========
}

void PausedState::handleInput() {
    // =========== Handle pause menu input (keyboard + mouse) [START] ===========
    sf::Event e{};
    auto& window = _game.getWindow();

    auto pickOptionAt = [&](int px, int py) -> int {
        const sf::Vector2f m = window.mapPixelToCoords({px, py});
        for (int i = 0; i < static_cast<int>(_options.size()); ++i) {
            if (_options[i].getGlobalBounds().contains(m)) {
                return i;
            }
        }
        return -1;
    };

    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            window.close();
            return;
        }

        // =========== Resize handling [START] ===========
        if (e.type == sf::Event::Resized) {
            window.setView(
                sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));
            onResize(e.size.width, e.size.height);
            continue;
        }
        // =========== Resize handling [END] ===========

        // =========== Mouse hover select [START] ===========
        if (e.type == sf::Event::MouseMoved) {
            const int hovered = pickOptionAt(e.mouseMove.x, e.mouseMove.y);
            if (hovered != -1 && hovered != _selected) {
                _selected = hovered;
                updateSelectionVisuals();
            }
        }
        // =========== Mouse hover select [END] ===========

        // =========== Mouse click activate [START] ===========
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            const int clicked = pickOptionAt(e.mouseButton.x, e.mouseButton.y);
            if (clicked != -1) {
                if (clicked != _selected) {
                    _selected = clicked;
                    updateSelectionVisuals();
                }
                activateSelected();
                return;
            }
        }
        // =========== Mouse click activate [END] ===========

        // =========== Keyboard handling [START] ===========
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Escape) {
                _state_manager.popState();
                return;
            }

            if (e.key.code == sf::Keyboard::W || e.key.code == sf::Keyboard::Up) {
                _selected = (_selected - 1 + static_cast<int>(_options.size())) % static_cast<int>(_options.size());
                updateSelectionVisuals();
            } else if (e.key.code == sf::Keyboard::S || e.key.code == sf::Keyboard::Down) {
                _selected = (_selected + 1) % static_cast<int>(_options.size());
                updateSelectionVisuals();
            } else if (e.key.code == sf::Keyboard::Enter) {
                activateSelected();
                return;
            }
        }
        // =========== Keyboard handling [END] ===========
    }
    // =========== Handle pause menu input (keyboard + mouse) [END] ===========
}

} // namespace representation
