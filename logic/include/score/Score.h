//
// Created by abdellah on 12/11/2025.
//

#ifndef PACMANPROJECT_SCORE_H
#define PACMANPROJECT_SCORE_H

#include <set>
#include <string>

#include "patterns/Observer.h"
#include "utils/Stopwatch.h"

namespace logic {

/**
 * @brief Tracks the current run score and manages the top-5 scoreboard on disk.
 *
 * This class is an Observer: it reacts to gameplay events (coin eaten, fruit eaten,
 * ghost eaten, death, etc.) and updates the running score accordingly.
 *
 * It also maintains a sorted set of high scores and can load/save them from/to
 * a scoreboard file.
 */
class Score final : public Observer {
public:
    /**
     * @brief Construct a score tracker and load existing high scores from disk.
     *
     * @param filePath   Path to the scoreboard file (relative or absolute).
     * @param startScore Starting score for the current run (useful when continuing
     *                   to the next level and carrying score forward).
     */
    explicit Score(std::string filePath = "scoreboard.txt", int startScore = 0);

    /**
     * @brief React to a gameplay event and update score/high-score state.
     * @param event Event type emitted by the game logic.
     */
    void onNotify(const EventType& event) override;

    /**
     * @brief Get the current in-game score for this run.
     * @return Current score value.
     */
    [[nodiscard]] int getScore() const;

    /**
     * @brief Update time-based score mechanics (bonuses/decay).
     *
     * Some scoring systems reward fast coin streaks or decay a bonus over time.
     * This method should be called once per frame to keep timers accurate.
     *
     * @param dt Delta time in seconds.
     */
    void update(float dt);

    /**
     * @brief Save the current high-score list to disk.
     *
     * Writes the stored high scores into the scoreboard file.
     */
    void saveHighScores() const;

    /**
     * @brief Load the high-score list from disk into memory.
     *
     * If the file does not exist, the high-score set remains empty.
     */
    void loadHighScores();

    /**
     * @brief Update the high-score list using the current run score.
     *
     * Typically called when a run ends to insert the final score into the
     * high-score set and keep only the top entries (e.g., top 5).
     */
    void updateHighScores();

    /**
     * @brief Convenience non-const score getter (prefer the const overload).
     * @return Current score value.
     */
    int getScore() { return _current_score; }

private:
    /** Current score accumulated in the ongoing run. */
    int _current_score = 0;

    /** Seconds since the last coin was eaten (used for streak/bonus logic). */
    float _time_since_last_coin = 0.f;

    /** Accumulator for time-based bonus decay or periodic updates. */
    float _decay_accum = 0.f;

    /** Sorted set of high scores (ascending order by default for std::set). */
    std::set<int> _high_scores;

    /** Path to the scoreboard file used for load/save operations. */
    std::string _score_file_path;

    /**
     * @brief Add points to the current score.
     * @param amount Number of points to add (should be non-negative).
     */
    void increaseScore(int amount);

    /**
     * @brief Compute the bonus points for eating a coin (if your rules allow it).
     *
     * This usually depends on timing (e.g., fast streaks) or combo mechanics.
     *
     * @return Bonus points to be added on top of the base coin score.
     */
    [[nodiscard]] int computeCoinBonus() const;
};

} // namespace logic

#endif // PACMANPROJECT_SCORE_H
