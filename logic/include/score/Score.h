//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_SCORE_H
#define PACMANPROJECT_SCORE_H
#include <set>
#include <string>

#include "patterns/Observer.h"
#include "utils/Stopwatch.h"

namespace logic {
class Score : public Observer {
public:
    explicit Score(std::string filePath = "leaderboard.txt");

    void onNotify(const EventType& event) override;

    [[nodiscard]] int getScore() const;

    void update(float dt);

    void saveHighScores() const;

    void loadHighScores();

    void updateHighScores();

    [[nodiscard]] const std::set<int>& getHighScores() const;

    int getScore() { return _current_score; }

private:
    int _current_score = 0;
    float _time_since_last_coin = 0.f;

    bool _new_score_added = false;

    std::set<int> _high_scores;

    std::string _score_file_path;

    void increaseScore(int amount);

    [[nodiscard]] int computeCoinBonus() const;

    [[nodiscard]] std::string getScoreFormat() const;
};
} // namespace logic

#endif // PACMANPROJECT_SCORE_H
