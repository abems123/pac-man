//
// Created by abdellah on 12/11/25.
//

#include "../../include/score/Score.h"

#include <algorithm>
#include <chrono>
#include <utility>

#include "events/Event.h"
#include "utils/FileReader.h"

namespace logic {
    Score::Score(std::string filePath) :_score_file_path(std::move(filePath)) {
        loadHighScores();
    }

    void Score::onNotify(const EventType &event) {
        switch (event) {
            case EventType::CoinEaten:
                increaseScore(computeCoinBonus());
                _time_since_last_coin = 0.f;
                break;
            case EventType::GhostEaten:
                increaseScore(100);
                break;
            case EventType::FruitEaten:
                increaseScore(100);
                break;
            case EventType::LevelCleared:
                increaseScore(500);
                updateHighScores();
                break;
            case EventType::Died:
                updateHighScores();
                break;
            default:
                break;
        }
    }

    int Score::getScore() const {
        return _current_score;
    }

    void Score::update(float dt) {

    }

    void Score::saveHighScores() const {
        std::string content;
        for (auto s = _high_scores.rbegin(), end = (_high_scores.size() > 5
                                                        ? std::prev(_high_scores.rend())
                                                        : _high_scores.rend()); s != end; ++s) {
            content += std::to_string(*s + '\n');
        }
        FileWriter::writeFile("scoreboard.txt", content);
    }

    void Score::loadHighScores() {
        const auto lines = FileReader::getFileLines(_score_file_path, true);

        for (auto &line: lines) {
            _high_scores.insert(std::stoi(line));
        }
    }

    void Score::updateHighScores() {
        if (_current_score <= *_high_scores.rbegin() && !_high_scores.empty()) return;

        _high_scores.insert(_current_score);
        saveHighScores();
    }

    const std::set<int> &Score::getHighScores() const {
        return _high_scores;
    }

    void Score::increaseScore(const int amount) {
        _current_score += amount;
    }

    int Score::computeCoinBonus() const {
        if (_time_since_last_coin <= .5f)
            return 50;
        if (_time_since_last_coin <= 1.f)
            return 20;
        return 10;
    }

    std::string Score::getScoreFormat() const {
        const auto now = std::chrono::system_clock::now();
        const auto seconds = std::chrono::floor<std::chrono::seconds>(now);

        const std::chrono::year_month_day date{
            std::chrono::floor<std::chrono::days>(seconds)
        };

        const std::chrono::hh_mm_ss time{
            seconds - std::chrono::floor<std::chrono::days>(seconds)
        };

        // Extract parts
        const int year = static_cast<int>(date.year());
        const unsigned month = static_cast<unsigned>(date.month());
        const unsigned day = static_cast<unsigned>(date.day());

        const unsigned hour = time.hours().count();
        const unsigned minute = time.minutes().count();

        const std::string time_str = std::to_string(day) + '/' + std::to_string(month) + std::to_string(year) + ' ' +
                                     std::to_string(hour) + ':' + std::to_string(minute);

        return time_str + " -> " + std::to_string(_current_score);
    }
}
