//
// Created by abdellah on 12/12/2025.
//

#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace representation {

/**
 * @brief Overlay state shown when the player completes a level.
 */
class VictoryState final : public State {
    /**
     * @brief Text displaying the number of lives carried into the next level.
     */
    sf::Text _lives_text;

    /**
     * @brief Soft shadow behind the main panel to add depth.
     */
    sf::RectangleShape _panel_shadow;

    /**
     * @brief Main UI panel (the "card") that contains the victory information.
     */
    sf::RectangleShape _panel;

    /**
     * @brief Fullscreen dim overlay behind the panel.
     */
    sf::RectangleShape _overlay;

    /** @brief Main title text ("VICTORY"). */
    sf::Text _title;

    /**
     * @brief Menu option texts (e.g., "Next Level", "Menu").
     */
    std::vector<sf::Text> _options;

    /**
     * @brief Index of the currently selected option in _options.
     */
    int _selected{0};

    /**
     * @brief Level that was just completed.
     */
    int _level;

    /**
     * @brief Lives remaining at the moment of victory.
     */
    int _carry_lives{};

    /**
     * @brief Final score at the moment the level was completed.
     */
    int _final_score{0};

    /** @brief Text that displays the final score (e.g., "Score: 1234"). */
    sf::Text _score_text;

    /**
     * @brief Build and position all UI elements.
     */
    void buildUi();

    /**
     * @brief Update option colors/styles to match the current selection.
     */
    void refreshSelection();

    /**
     * @brief Execute the action of the currently selected option.
     */
    void activate() const;

public:
    /**
     * @brief Construct the victory overlay for a completed level.
     *
     * @param manager   State manager used for stack transitions.
     * @param game      Game instance (window/font access).
     * @param level     Level that was completed.
     * @param score     Score at the moment of victory.
     * @param livesLeft Lives remaining at the moment of victory.
     */
    VictoryState(StateManager& manager, Game& game, int level, int score, int livesLeft);

    /**
     * @brief Handle victory menu input (navigation + confirm + close).
     */
    void handleInput() override;

    /**
     * @brief Update overlay animations/timers (optional).
     *
     * @param dt Delta time in seconds.
     */
    void update(float dt) override;

    /**
     * @brief Render the victory overlay on top of the underlying game.
     *
     * @param window Render target.
     */
    void render(sf::RenderWindow& window) override;

    /**
     * @brief Victory screen is drawn over the level instead of replacing it.
     * @return True (overlay).
     */
    bool isOverlay() const override { return true; }

    /**
     * @brief Re-layout the victory overlay after a window resize.
     *
     * @param width  New window width in pixels.
     * @param height New window height in pixels.
     */
    void onResize(unsigned width, unsigned height) override;
};

} // namespace representation
