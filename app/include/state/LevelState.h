//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_LEVELSTATE_H
#define PACMANPROJECT_LEVELSTATE_H
#include <memory>

#include "State.h"
#include "world/World.h"


namespace representation {
    class LevelState : public State {
        // std::unique_ptr<logic::World> world;

    public:
        using State::State;

        void handleInput() override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) override;
    };
}

#endif //PACMANPROJECT_LEVELSTATE_H
