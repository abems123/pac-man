//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_PAUSEDSTATE_H
#define PACMANPROJECT_PAUSEDSTATE_H
#include "State.h"

namespace representation {
    class PausedState : public State{
    public:
        void handleInput() override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) override;

        void onEnter() override;

        void onExit() override;

        using State::State;

    };
}

#endif //PACMANPROJECT_PAUSEDSTATE_H