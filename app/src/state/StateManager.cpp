//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/StateManager.h"

#include <memory>

#include "state/LevelState.h"
#include "state/MenuState.h"

namespace representation {
void StateManager::applyPending() {
    // =========== Apply queued stack changes safely [START] ===========
    if (_pending_pop_to_menu) {
        while (_states.size() > 1)
            _states.pop();
        _pending_pop_to_menu = false;
        _pending_pops = 0;
    } else {
        while (_pending_pops > 0 && !_states.empty()) {
            _states.pop();
            --_pending_pops;
        }
    }

    for (auto& s : _pending_push)
        _states.push(std::move(s));
    _pending_push.clear();
    // =========== Apply queued stack changes safely [END] ===========
}

void StateManager::pushState(std::unique_ptr<State> state) {
    // =========== Push now or queue if dispatching [START] ===========
    if (!state)
        return;

    if (_dispatching) {
        _pending_push.push_back(std::move(state));
        return;
    }

    _states.push(std::move(state));
    // =========== Push now or queue if dispatching [END] ===========
}

void StateManager::popState() {
    // =========== Pop now or queue if dispatching [START] ===========
    if (_dispatching) {
        ++_pending_pops;
        return;
    }

    if (!_states.empty())
        _states.pop();
    // =========== Pop now or queue if dispatching [END] ===========
}

void StateManager::notifyResize(const unsigned width, const unsigned height) {
    // =========== Broadcast resize to every state in the stack [START] ===========
    std::stack<std::unique_ptr<State>> tmp;

    // Move out (top -> bottom), notify, push into tmp
    while (!_states.empty()) {
        std::unique_ptr<State> s = std::move(_states.top());
        _states.pop();

        if (s) {
            s->onResize(width, height);
        }

        tmp.push(std::move(s));
    }

    // Restore original order back into _states
    while (!tmp.empty()) {
        _states.push(std::move(tmp.top()));
        tmp.pop();
    }
    // =========== Broadcast resize to every state in the stack [END] ===========
}


void StateManager::popToMenu() {
    // =========== Pop-to-menu now or queue if dispatching [START] ===========
    if (_dispatching) {
        _pending_pop_to_menu = true;
        return;
    }

    while (_states.size() > 1)
        _states.pop();
    // =========== Pop-to-menu now or queue if dispatching [END] ===========
}

void StateManager::handleInput() {
    // =========== Dispatch input to active state [START] ===========
    if (_states.empty())
        return;

    _dispatching = true;
    _states.top()->handleInput();
    _dispatching = false;

    applyPending();
    // =========== Dispatch input to active state [END] ===========
}

void StateManager::update(float dt) {
    // =========== Update active state [START] ===========
    if (_states.empty())
        return;

    _dispatching = true;
    _states.top()->update(dt);
    _dispatching = false;

    applyPending();
    // =========== Update active state [END] ===========
}

void StateManager::render(sf::RenderWindow& window) {
    if (_states.empty())
        return;

    // =========== Render state below overlay [START] ===========
    if (_states.top()->isOverlay() && _states.size() >= 2) {
        auto top = std::move(_states.top());
        _states.pop();

        _states.top()->render(window); // draw the level behind

        _states.push(std::move(top));
    }
    // =========== Render state below overlay [END] ===========

    _states.top()->render(window);
}

} // namespace representation
