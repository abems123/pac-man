//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_MENUSTATE_H
#define PACMANPROJECT_MENUSTATE_H
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"


namespace representation {
    class Game;

    class MenuState : public State {
        sf::Font font;
        sf::Text titleText;
        sf::Text instructionText;

        float blinkTimer = 0.f;
        bool visible = true;

    public:
        MenuState();

        MenuState(StateManager &manager, Game &game);

        void handleInput() override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) override;

        void onEnter() override;

        void onExit() override;
    };
}
#endif //PACMANPROJECT_MENUSTATE_H
