//
// Created by Abdellah on 12/27/2025.
//

#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string>
#include <vector>

namespace representation {

/**
 * @brief Overlay state shown when the player loses all lives.
 *
 * Displays the final score and level reached
 * by comparing against the saved scoreboard file.
 */
class GameOverState final : public State {
public:
    /**
     * @brief Construct the GameOver screen.
     * @param manager State manager that owns the state stack.
     * @param game Game instance.
     * @param level Level reached when the run ended.
     * @param score Final score of the run.
     */
    GameOverState(StateManager& manager, Game& game, int level, int score);

    /**
     * @brief Handle user input (menu navigation + confirm).
     */
    void handleInput() override;

    /**
     * @brief Render the overlay on top of the previous state.
     * @param window Render target.
     */
    void render(sf::RenderWindow& window) override;

    /**
     * @brief Indicates this state should be drawn over the underlying game.
     * @return True (overlay).
     */
    bool isOverlay() const override { return true; }

private:
    /**
     * @brief Build the overlay UI elements (texts, positions, options).
     */
    void buildUi();

    /**
     * @brief Refresh option highlight based on current selection.
     */
    void refreshSelection();

    /**
     * @brief Execute the currently selected option.
     */
    void activate() const;

    /**
     * @brief Rebuild and re-layout the Game Over overlay after a window resize.
     * @param width  New window width in pixels.
     * @param height New window height in pixels.
     */
    void onResize(unsigned width, unsigned height) override;

private:
    /** Level reached when the run ended. */
    int _level = 1;

    /** Final score for this run. */
    int _final_score = 0;

    /** Currently selected menu option index. */
    int _selected = 0;

    /** Dark transparent background overlay. */
    sf::RectangleShape _overlay;

    /** "GAME OVER" title text. */
    sf::Text _title;

    /** Level display text. */
    sf::Text _level_text;

    /** Score display text. */
    sf::Text _score_text;

    /** Optional "NEW HIGH SCORE" text. */
    sf::Text _high_score_text;

    /** Menu option texts. */
    std::vector<sf::Text> _options;

    /** Path to the scoreboard file. */
    std::string _scoreboard_path = "scoreboard.txt";
};

} // namespace representation
