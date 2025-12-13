//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/StateManager.h"

#include <memory>

#include "state/LevelState.h"
#include "state/MenuState.h"

namespace representation {
    StateManager::StateManager() {
        // the StateManager will start with an initial state of MenuState
        // const auto menu_state = std::make_unique<MenuState>();
        // states.push(menu_state.get());
    }

    void StateManager::handleInput() {
        if (!states.empty())
            states.top()->handleInput();
    }

    void StateManager::update(const float dt) {
        if (!states.empty())
            states.top()->update(dt);
    }

    void StateManager::render(sf::RenderWindow& window) {
        if (!states.empty())
            states.top()->render(window);
    }

}
