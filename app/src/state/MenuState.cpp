//
// Created by abdellah on 12/12/25.
//

#include "state/MenuState.h"
#include <SFML/Graphics.hpp>

#include "game/Game.h"
#include "state/LevelState.h"
#include "utils/ResourceManager.h"

namespace representation {
    MenuState::MenuState(StateManager &manager, Game &game)
        : State(manager, game) {
        // Load font from Game resource manager (or directly load it here)
        // Adjust according to how you store fonts.
        font = game.getResources().getFont(Font::Main_Font);

        titleText.setFont(font);
        titleText.setString("PAC-MAN");
        titleText.setCharacterSize(90);
        titleText.setFillColor(sf::Color::Yellow);

        // Center title on screen
        auto size = game.getWindow().getSize();
        titleText.setPosition(
            size.x / 2.f - titleText.getLocalBounds().width / 2.f,
            size.y * 0.25f
        );

        instructionText.setFont(font);
        instructionText.setString("Press ENTER to Start");
        instructionText.setCharacterSize(32);
        instructionText.setFillColor(sf::Color(255, 255, 255, 200));

        instructionText.setPosition(
            size.x / 2.f - instructionText.getLocalBounds().width / 2.f,
            size.y * 0.55f
        );
    }

    void MenuState::handleInput() {
        sf::Event event{};
        auto &window = game.getWindow();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    // Transition to LevelState
                    stateManager.pushState(std::make_unique<LevelState>(stateManager, game));
                }
            }
        }
    }

    void MenuState::update(float dt) {
        blinkTimer += dt;

        constexpr float blinkSpeed = 1.0f;

        if (blinkTimer >= blinkSpeed) {
            blinkTimer = 0.f;
            visible = !visible;
        }

        // Adjust alpha of the text
        sf::Color c = instructionText.getFillColor();
        c.a = visible ? 255 : 60;   // 255 = fully visible, 60 = dimmed
        instructionText.setFillColor(c);
    }


    void MenuState::render(sf::RenderWindow &window) {
        window.draw(titleText);
        window.draw(instructionText);
    }

    void MenuState::onEnter() {
        // Play menu music or reset menu animations if needed
    }

    void MenuState::onExit() {
        // Stop music or cleanup UI transitions if needed
    }
} // namespace representation
