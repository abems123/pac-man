//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_STATEMANAGER_H
#define PACMANPROJECT_STATEMANAGER_H
#include <memory>
#include <stack>

#include "State.h"

#include <vector>

namespace sf {
class RenderWindow;
}

namespace representation {
class StateManager {
    /** Stack of states; top() is the active state. */
    std::stack<std::unique_ptr<State>> _states;

    /** True while a state callback is running (handleInput/update/render). */
    bool _dispatching{false};

    /** Pops requested during dispatching; applied afterwards. */
    int _pending_pops{0};

    /** If true, pop-to-menu is requested during dispatching; applied afterwards. */
    bool _pending_pop_to_menu{false};

    /** Pushes requested during dispatching; applied afterwards. */
    std::vector<std::unique_ptr<State>> _pending_push;

    /** Applies queued stack changes after the current state call finishes.
     *  @return Nothing.
     */
    void applyPending();

public:
    StateManager() = default;

    /**
     * @brief Notify all states that the window was resized.
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     */
    void notifyResize(unsigned width, unsigned height);

    /** Pushes a state on top of the stack.
     *  @param state State to push (ownership transferred).
     */
    void pushState(std::unique_ptr<State> state);

    /** Pops the top state. */
    void popState();

    /** Pops until only the bottom state remains (MenuState). */
    void popToMenu();

    /** Only the top state handles input.
     *  @return Nothing.
     */
    void handleInput();

    /** Updates states respecting pause/settings overlay blocking.
     *  @param dt Delta time in seconds.
     *  @return Nothing.
     */
    void update(float dt);

    /** Renders states respecting overlay transparency.
     *  @param window Render target.
     *  @return Nothing.
     */
    void render(sf::RenderWindow& window);
};
} // namespace representation

#endif // PACMANPROJECT_STATEMANAGER_H
