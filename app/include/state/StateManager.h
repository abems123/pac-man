//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_STATEMANAGER_H
#define PACMANPROJECT_STATEMANAGER_H
#include <memory>
#include <stack>

#include "State.h"


namespace sf {
    class RenderWindow;
}

namespace representation {
    class StateManager {
        std::stack<std::unique_ptr<State> > states;

    public:
        StateManager();

        void handleInput();

        void pushState(std::unique_ptr<State> state) { states.push(std::move(state)); }

        void popState() { states.pop(); };

        void update(float dt);

        void render(sf::RenderWindow &window);
    };
}


#endif //PACMANPROJECT_STATEMANAGER_H
