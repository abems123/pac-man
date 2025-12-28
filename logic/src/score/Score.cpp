//
// Created by abdellah on 12/11/25.
//

#include "../../include/score/Score.h"

#include <algorithm>
#include <chrono>
#include <utility>

#include <events/EventType.h>
#include "utils/FileManager.h"

#include <utils/Constants.h>

namespace logic {
Score::Score(std::string filePath, const int startScore) : _current_score(startScore), _score_file_path(std::move(filePath)) {
    loadHighScores();
}

void Score::onNotify(const EventType& event) {
    switch (event) {
    case EventType::CoinEaten:
        increaseScore(computeCoinBonus());
        _time_since_last_coin = 0.f;
        break;
    case EventType::GhostEaten:
        increaseScore(Constants::GHOST_SCORE);
        break;
    case EventType::FruitEaten:
        increaseScore(Constants::FRUIT_SCORE);
        break;
    case EventType::LevelCleared:
        increaseScore(Constants::LEVEL_WIN_SCORE);
        updateHighScores();
        break;
    case EventType::Died:
        updateHighScores();
        break;
    default:
        break;
    }
}

int Score::getScore() const { return _current_score; }


void Score::update(float dt) {
    dt = std::max(0.f, dt);

    _time_since_last_coin += dt;

    // Decrease score over time: 10 point per second
    _decay_accum += dt;
    while (_decay_accum >= 1.f) {
        _current_score = std::max(0, _current_score - Constants::SCORE_DECAY);
        _decay_accum -= 1.f;
    }
}


void Score::saveHighScores() const {
    std::string content;

    int count = 0;
    for (auto it = _high_scores.rbegin(); it != _high_scores.rend() && count < 5; ++it, ++count) {
        content += std::to_string(*it);
        content += '\n';
    }

    FileWriter::writeFile(_score_file_path, content);
}


void Score::loadHighScores() {
    for (const auto lines = FileReader::getFileLines(_score_file_path, true); auto& line : lines) {
        _high_scores.insert(std::stoi(line));
    }
}

void Score::updateHighScores() {
    // Ignore if score doesn't enter top 5
    if (_high_scores.size() >= 5 && _current_score <= *_high_scores.begin())
        return;

    _high_scores.insert(_current_score);

    // Keep only top 5 and remove smallest
    while (_high_scores.size() > 5)
        _high_scores.erase(_high_scores.begin());

    saveHighScores();
}



void Score::increaseScore(const int amount) { _current_score += amount; }

int Score::computeCoinBonus() const {
    if (_time_since_last_coin <= .5f)
        return Constants::COIN_SCORE_FASTEST;
    if (_time_since_last_coin <= 2.f)
        return Constants::COIN_SCORE_FAST;
    return Constants::COIN_SCORE_SLOW;
}

} // namespace logic
