//
// Created by abdellah on 12/20/2025.
//

#pragma once

namespace logic {
struct Constants {
    /** Points gained when Pac-Man eats a fruit. */
    static constexpr int FRUIT_SCORE = 100;

    /** Points gained when Pac-Man eats a coin in less than 0.5 seconds since the last one. */
    static constexpr int COIN_SCORE_FASTEST = 20;

    /** Points gained when Pac-Man eats a coin in less than 2 seconds since the last one. */
    static constexpr int COIN_SCORE_FAST = 10;

    /** Points gained when Pac-Man eats a coin after 2 seconds since the last one. */
    static constexpr int COIN_SCORE_SLOW = 5;

    /** Points gained when Pac-Man eats a ghost. */
    static constexpr int GHOST_SCORE = 50;

    /** Points gained when Pac-Man clears a level. */
    static constexpr int LEVEL_WIN_SCORE = 200;

    /** Points lost every second because of decay. */
    static constexpr int SCORE_DECAY = 10;

    /** Pac-Man movement speed. */
    static constexpr float PACMAN_SPEED = 1.f;

    /** Ghost movement speed at level 1 (before any difficulty scaling). */
    static constexpr float GHOST_START_SPEED = 0.1f;

    /** How much faster ghosts get each level after you win (as a fraction).
     *  0.10f means +10% per level.
     */
    static constexpr float GHOST_SPEED_BOOST_PERCENTAGE = 0.1f;

    /** How much shorter frightened mode becomes each level after you win (as a fraction).
     *  0.10f means -10% per level.
     */
    static constexpr float FRIGHTENED_TIME_REDUCTION_PERCENTAGE = 0.1f;

    /** Ghost speed multiplier during frightened mode (0.7f = 70% of normal speed). */
    static constexpr float FRIGHTENED_MODE_SLOW_PERCENTAGE = 0.7f;

    /** Last part of frightened mode where ghosts flash (fraction of total frightened time). */
    static constexpr float FLASHING_PERCENTAGE = 0.3f;

    /** Seconds of invulnerability/pause after Pac-Man gets hit before play resumes. */
    static constexpr float HIT_COOLDOWN = 3.f;

    /** Frightened mode duration at level 1 (before difficulty scaling). */
    static constexpr float FRIGHTENED_DURATION = 10.f;
};
} // namespace logic
