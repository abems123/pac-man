//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_STATE_H
#define PACMANPROJECT_STATE_H

namespace sf {
class RenderWindow;
}

namespace representation {
class Game;
class StateManager;

class State {
protected:
    StateManager& stateManager;
    Game& game;

public:
    explicit State(StateManager& manager, Game& game) : stateManager(manager), game(game) {}

    State(State& other);

    virtual ~State() = default;

    // Each State handles input differently (Menu → Start, Level → Movement, Pause → Resume)
    virtual void handleInput() = 0;

    // Each State updates its own behavior (Level → World update, Pause → no gameplay update)
    virtual void update(float dt) = 0;

    // Each State draws its own screen (Menu screen, gameplay screen, pause overlay, victory screen)
    virtual void render(sf::RenderWindow& window) = 0;

    // Optional: lifecycle hooks when State is pushed or popped (not required by assignment but useful)
    virtual void onEnter() {}

    virtual void onExit() {}
};
} // namespace representation

#endif // PACMANPROJECT_STATE_H
