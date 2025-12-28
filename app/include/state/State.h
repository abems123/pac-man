//
// Created by abdellah on 12/12/2025.
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
    StateManager& _state_manager;
    Game& _game;

public:
    /**
     * @brief Construct a State.
     * @param manager The StateManager that owns the stack.
     * @param game The Game instance (window, resources, etc.).
     */
    explicit State(StateManager& manager, Game& game) : _state_manager(manager), _game(game) {}

    /** @brief Virtual destructor. */
    virtual ~State() = default;

    /** @brief Handles user input for this state (keyboard/mouse/window events). */
    virtual void handleInput() = 0;

    /**
     * @brief Updates this state's logic.
     * @param dt Delta time in seconds.
     */
    virtual void update(float dt) {}

    /**
     * @brief Renders this state.
     * @param window Window to draw to.
     */
    virtual void render(sf::RenderWindow& window) = 0;

    /** @brief If true, states below should still be drawn (pause/settings overlay). */
    virtual bool isOverlay() const { return false; }

    /**
     * @brief Called when the window size changed.
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     *
     * Default does nothing. States override this to re-layout UI / camera-dependent positioning.
     */
    virtual void onResize(unsigned width, unsigned height) {}
};

} // namespace representation

#endif // PACMANPROJECT_STATE_H
