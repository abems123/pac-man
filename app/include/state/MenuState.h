//
// Created by abdellah on 12/12/25.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <filesystem>
#include <string>

#include "state/State.h"

namespace representation {

/**
 * @brief Main menu state displaying title, top scores, and start + audio toggles.
 */
class MenuState final : public State {
public:
    /**
     * @brief Construct the menu state.
     * @param manager State manager that owns the stack.
     * @param game Game instance (window, resources, etc.).
     */
    MenuState(StateManager& manager, Game& game);

    /**
     * @brief Handle user input in the menu (Enter to play, resize, close, toggle music).
     */
    void handleInput() override;

    /**
     * @brief Update menu logic.
     * @param dt Delta time in seconds.
     *
     * Only refreshes the scoreboard if the scoreboard file changed since last time.
     */
    void update(float dt) override;

    /**
     * @brief Render the menu UI.
     * @param window Render window to draw into.
     */
    void render(sf::RenderWindow& window) override;

    /**
     * @brief Re-layout menu UI after a window resize.
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     */
    void onResize(unsigned width, unsigned height) override;

private:
    // ===================== Music toggle UI =====================

    /**
     * @brief Cached music enabled value for button label (backed by Game::isMusicEnabled()).
     */
    bool _music_enabled{true};

    /**
     * @brief Music toggle button background.
     */
    sf::RectangleShape _music_btn{};

    /**
     * @brief Shadow rectangle for the music toggle button.
     */
    sf::RectangleShape _music_btn_shadow{};

    /**
     * @brief Music toggle button label.
     */
    sf::Text _music_btn_text{};

    /**
     * @brief Shadow label for the music toggle button.
     */
    sf::Text _music_btn_text_shadow{};

    /**
     * @brief Update the text displayed on the music toggle button.
     *
     * Example: "MUSIC: ON" / "MUSIC: OFF"
     */
    void updateMusicButtonText();

    /**
     * @brief Position and size the music toggle button based on current window size.
     */
    void layoutMusicButton();

    /**
     * @brief Toggle global music enabled state and update button visuals.
     */
    void toggleMusic();

    // ===================== UI helpers =====================

    /**
     * @brief Center text around (x,y) using its local bounds.
     * @param t Text to center.
     * @param x Target x coordinate (pixels).
     * @param y Target y coordinate (pixels).
     */
    void centerText(sf::Text& t, float x, float y);

    /**
     * @brief Build all UI elements (scoreboard + title + start hint + music button).
     */
    void buildUi();

    /**
     * @brief Read the scoreboard file and fill the top 5 scores array.
     */
    void refreshTopScores();

    /**
     * @brief Layout the scoreboard “card” and its rows.
     */
    void layoutScoreboard();

    /**
     * @brief Layout the “Press Enter to start playing” hint.
     */
    void layoutStartHint();

    /**
     * @brief Layout the yellow “PAC MAN” title above the scoreboard.
     */
    void layoutTitle();

    /**
     * @brief Refresh top scores only if the scoreboard file changed since last check.
     */
    void refreshTopScoresIfFileChanged();

    /**
     * @brief Check whether the scoreboard file changed since last check.
     * @return True if the file is new/changed/missing compared to cached state.
     */
    bool scoreboardFileChanged();

private:
    // ===================== Scoreboard data =====================

    /**
     * @brief Path to the scoreboard file that stores high scores.
     */
    std::string _scoreboard_path = "scoreboard.txt";

    /**
     * @brief Cached top 5 scores (descending). -1 means “no entry yet”.
     */
    std::array<int, 5> _top_scores{-1, -1, -1, -1, -1};

    // ===================== File change cache =====================

    /**
     * @brief Cached last write time of the scoreboard file (used to detect changes).
     */
    std::filesystem::file_time_type _scoreboard_last_write{};

    /**
     * @brief Cached file size of the scoreboard file (used to detect changes).
     */
    std::uintmax_t _scoreboard_last_size = 0;

    /**
     * @brief True once we have successfully cached an initial file state.
     */
    bool _scoreboard_cache_valid = false;

    // ===================== UI elements =====================

    /**
     * @brief Scoreboard card shadow rectangle.
     */
    sf::RectangleShape _score_card_shadow{};

    /**
     * @brief Scoreboard card background rectangle.
     */
    sf::RectangleShape _score_card{};

    /**
     * @brief "TOP SCORES" heading text.
     */
    sf::Text _score_heading{};

    /**
     * @brief Score row texts (rank + value).
     */
    std::array<sf::Text, 5> _score_rows{};

    /**
     * @brief Shadow texts for each score row.
     */
    std::array<sf::Text, 5> _score_rows_shadow{};

    /**
     * @brief Start hint text (e.g., "Press Enter to start playing").
     */
    sf::Text _start_hint{};

    /**
     * @brief Shadow for the start hint text.
     */
    sf::Text _start_hint_shadow{};

    /**
     * @brief Big “PAC MAN” title text.
     */
    sf::Text _title{};

    /**
     * @brief Shadow for the “PAC MAN” title.
     */
    sf::Text _title_shadow{};
};

} // namespace representation
