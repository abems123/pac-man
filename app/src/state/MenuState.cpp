//
// Created by abdellah on 12/12/25.
//

#include "../../include/state/MenuState.h"

#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <fstream>
#include <game/Game.h>
#include <state/LevelState.h>
#include <vector>

#include <utils/ResourceManager.h>

namespace representation {

// =========== Center text helper [START] ===========
void MenuState::centerText(sf::Text& t, const float x, const float y) {
    const sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
    t.setPosition(x, y);
}
// =========== Center text helper [END] ===========

MenuState::MenuState(StateManager& manager, Game& game) : State(manager, game) { buildUi(); }

// =========== Music toggle helpers [START] ===========
void MenuState::updateMusicButtonText() {
    _music_enabled = _game.isMusicEnabled();

    _music_btn_text.setString(_music_enabled ? "MUSIC: ON" : "MUSIC: OFF");
    _music_btn_text_shadow = _music_btn_text;
    _music_btn_text_shadow.setFillColor(sf::Color(0, 0, 0, 160));
}

void MenuState::toggleMusic() {
    const bool new_state = !_game.isMusicEnabled();
    _game.setMusicEnabled(new_state);

    updateMusicButtonText();
    layoutMusicButton();

    if (new_state) {
        _game.playMusic(Game::MusicTrack::Menu, true);
    }
}

void MenuState::layoutMusicButton() {
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);

    auto& rm = ResourceManager::instance();
    const auto& font = rm.getFont(Font::SuperMeatBall);

    // Text style
    _music_btn_text.setFont(font);
    _music_btn_text.setCharacterSize(static_cast<unsigned>(std::clamp(H * 0.045f, 14.f, 28.f)));
    _music_btn_text.setFillColor(sf::Color::White);

    updateMusicButtonText();

    // Button sizing from text bounds
    const sf::FloatRect tb = _music_btn_text.getLocalBounds();
    const float padX = std::clamp(W * 0.012f, 10.f, 18.f);
    const float padY = std::clamp(H * 0.010f, 8.f, 14.f);

    const float btnW = tb.width + padX * 2.f;
    const float btnH = tb.height + padY * 2.f;

    const float marginX = std::clamp(W * 0.03f, 14.f, 28.f);
    const float marginY = std::clamp(H * 0.03f, 14.f, 28.f);

    const float x = W - marginX - btnW;
    const float y = H - marginY - btnH;

    // Shadow
    _music_btn_shadow.setSize({btnW, btnH});
    _music_btn_shadow.setPosition(x + 3.f, y + 3.f);
    _music_btn_shadow.setFillColor(sf::Color(0, 0, 0, 120));

    // Button
    _music_btn.setSize({btnW, btnH});
    _music_btn.setPosition(x, y);
    _music_btn.setFillColor(sf::Color(15, 15, 20, 210));
    _music_btn.setOutlineThickness(std::max(2.f, H * 0.0025f));
    _music_btn.setOutlineColor(sf::Color(255, 255, 255, 80));

    // Center label inside button
    centerText(_music_btn_text_shadow, x + btnW * 0.5f + 2.f, y + btnH * 0.5f + 2.f);
    centerText(_music_btn_text, x + btnW * 0.5f, y + btnH * 0.5f);
}
// =========== Music toggle helpers [END] ===========

// =========== Build UI [START] ===========
void MenuState::buildUi() {
    // First time: force load + layout.
    refreshTopScores();
    layoutScoreboard();
    layoutTitle();
    layoutStartHint();

    // Cache current file state so update() can detect changes later.
    _scoreboard_cache_valid = false;
    refreshTopScoresIfFileChanged();

    layoutMusicButton();

    // Ensure menu music respects global setting
    if (_game.isMusicEnabled()) {
        _game.playMusic(Game::MusicTrack::Menu, true);
    }
}
// =========== Build UI [END] ===========

// =========== Check scoreboard file changed [START] ===========
bool MenuState::scoreboardFileChanged() {
    namespace fs = std::filesystem;

    std::error_code ec;

    // File missing?
    if (!fs::exists(_scoreboard_path, ec) || ec) {
        const bool changed = _scoreboard_cache_valid;
        _scoreboard_cache_valid = false;
        _scoreboard_last_size = 0;
        return changed;
    }

    const auto t = fs::last_write_time(_scoreboard_path, ec);
    if (ec) {
        // If we can't read write-time, be safe: treat as changed once.
        const bool changed = !_scoreboard_cache_valid;
        _scoreboard_cache_valid = true;
        return changed;
    }

    const auto sz = fs::file_size(_scoreboard_path, ec);
    if (ec) {
        const bool changed = !_scoreboard_cache_valid;
        _scoreboard_cache_valid = true;
        return changed;
    }

    // First time we see it -> treat as changed to force a load.
    if (!_scoreboard_cache_valid) {
        _scoreboard_cache_valid = true;
        _scoreboard_last_write = t;
        _scoreboard_last_size = sz;
        return true;
    }

    // Changed if timestamp or size differs.
    if (t != _scoreboard_last_write || sz != _scoreboard_last_size) {
        _scoreboard_last_write = t;
        _scoreboard_last_size = sz;
        return true;
    }

    return false;
}
// =========== Check scoreboard file changed [END] ===========

// =========== Refresh only if file changed [START] ===========
void MenuState::refreshTopScoresIfFileChanged() {
    if (!scoreboardFileChanged())
        return;

    refreshTopScores();
    layoutScoreboard();
    layoutTitle();
    layoutStartHint();
}
// =========== Refresh only if file changed [END] ===========

// =========== Update [START] ===========
void MenuState::update(float) {
    // Do NOT rebuild every frame.
    // Only refresh if the file changed since last time we checked.
    refreshTopScoresIfFileChanged();
}
// =========== Update [END] ===========

// =========== Layout title “PAC MAN” [START] ===========
void MenuState::layoutTitle() {
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);

    auto& rm = ResourceManager::instance();
    const auto& mainFont = rm.getFont(Font::Crackman);

    _title.setFont(mainFont);
    _title.setString("PAC MAN");
    _title.setCharacterSize(static_cast<unsigned>(H * 0.14f));
    _title.setFillColor(sf::Color(255, 215, 0));

    _title_shadow = _title;
    _title_shadow.setFillColor(sf::Color(0, 0, 0, 170));

    const float x = W * 0.5f;
    const float y = H * 0.18f; // above the card

    centerText(_title_shadow, x + 3.f, y + 3.f);
    centerText(_title, x, y);
}
// =========== Layout title “PAC MAN” [END] ===========

void MenuState::handleInput() {
    // =========== Handle menu input [START] ===========
    sf::Event e{};
    auto& window = _game.getWindow();

    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed)
            window.close();

        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
            _state_manager.pushState(std::make_unique<LevelState>(_state_manager, _game, 1, 0, 3));
            return;
        } else if (e.type == sf::Event::Resized) {
            window.setView(
                sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(e.size.width), static_cast<float>(e.size.height))));

            onResize(e.size.width, e.size.height);
        } else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::M) {
            toggleMusic();
        } else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2f m(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y));
            if (_music_btn.getGlobalBounds().contains(m)) {
                toggleMusic();
            }
        }
    }
    // =========== Handle menu input [END] ===========
}

void MenuState::render(sf::RenderWindow& window) {
    // Title
    window.draw(_title_shadow);
    window.draw(_title);

    // Scoreboard card
    window.draw(_score_card_shadow);
    window.draw(_score_card);
    window.draw(_score_heading);

    for (int i = 0; i < 5; ++i) {
        window.draw(_score_rows_shadow[i]);
        window.draw(_score_rows[i]);
    }

    // Start hint
    window.draw(_start_hint_shadow);
    window.draw(_start_hint);

    // Music button
    window.draw(_music_btn_shadow);
    window.draw(_music_btn);
    window.draw(_music_btn_text_shadow);
    window.draw(_music_btn_text);
}

void MenuState::onResize(const unsigned, const unsigned) {
    layoutScoreboard();
    layoutTitle();
    layoutStartHint();
    layoutMusicButton();
}

void MenuState::refreshTopScores() {
    // =========== Load top scores from file [START] ===========
    std::vector<int> scores;
    scores.reserve(16);

    std::ifstream in(_scoreboard_path);
    if (in.is_open()) {
        for (int s; (in >> s);) {
            scores.push_back(s);
        }
    }

    std::ranges::sort(scores, std::greater<>());

    for (int i = 0; i < 5; ++i) {
        _top_scores[i] = (i < static_cast<int>(scores.size())) ? scores[i] : -1;
    }
    // =========== Load top scores from file [END] ===========
}

void MenuState::layoutScoreboard() {
    // =========== Layout scoreboard card [START] ===========
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);

    auto& rm = ResourceManager::instance();
    const auto& mainFont = rm.getFont(Font::Crackman);
    const auto& secondaryFont = rm.getFont(Font::SuperMeatBall);

    // Card placement: centered, slightly below the top third
    const float cardW = W * 0.45f;
    const float cardH = H * 0.50f;

    const float cardX = W * 0.5f - cardW * 0.5f;
    const float cardY = H * 0.30f;

    // Shadow
    _score_card_shadow.setSize({cardW, cardH});
    _score_card_shadow.setPosition(cardX + W * 0.008f, cardY + H * 0.01f);
    _score_card_shadow.setFillColor(sf::Color(0, 0, 0, 120));

    // Card
    _score_card.setSize({cardW, cardH});
    _score_card.setPosition(cardX, cardY);
    _score_card.setFillColor(sf::Color(15, 15, 20, 210));
    _score_card.setOutlineThickness(std::max(2.f, H * 0.003f));
    _score_card.setOutlineColor(sf::Color(255, 255, 255, 80));

    // Heading
    _score_heading.setFont(mainFont);
    _score_heading.setCharacterSize(static_cast<unsigned>(H * 0.06f));
    _score_heading.setFillColor(sf::Color::White);
    _score_heading.setString("TOP SCORES");
    centerText(_score_heading, cardX + cardW * 0.5f, cardY + cardH * 0.14f);

    // Rows
    const float startY = cardY + cardH * 0.30f;
    const float rowGap = cardH * 0.12f;

    for (int i = 0; i < 5; ++i) {
        const int rank = i + 1;

        const std::string scoreStr = (_top_scores[i] >= 0) ? std::to_string(_top_scores[i]) : std::string("---");
        const std::string line = std::to_string(rank) + ".   " + scoreStr;

        _score_rows[i].setFont(secondaryFont);
        _score_rows[i].setCharacterSize(static_cast<unsigned>(H * 0.055f));
        _score_rows[i].setFillColor(sf::Color::White);
        _score_rows[i].setString(line);

        // Shadow (same text, offset)
        _score_rows_shadow[i] = _score_rows[i];
        _score_rows_shadow[i].setFillColor(sf::Color(0, 0, 0, 160));

        const float y = startY + i * rowGap;
        centerText(_score_rows_shadow[i], cardX + cardW * 0.5f + 2.f, y + 2.f);
        centerText(_score_rows[i], cardX + cardW * 0.5f, y);
    }
    // =========== Layout scoreboard card [END] ===========
}

void MenuState::layoutStartHint() {
    // =========== Layout "Press Enter" hint [START] ===========
    const auto win = _game.getWindow().getSize();
    const float W = static_cast<float>(win.x);
    const float H = static_cast<float>(win.y);

    auto& rm = ResourceManager::instance();
    const auto& font = rm.getFont(Font::SuperMeatBall);

    _start_hint.setFont(font);
    _start_hint.setCharacterSize(static_cast<unsigned>(H * 0.060f));
    _start_hint.setFillColor(sf::Color(255, 255, 255, 235));
    _start_hint.setString("Press Enter to start playing");
    _start_hint.setOutlineThickness(std::max(1.f, H * 0.0025f));
    _start_hint.setOutlineColor(sf::Color(0, 0, 0, 180));

    _start_hint_shadow = _start_hint;
    _start_hint_shadow.setFillColor(sf::Color(0, 0, 0, 160));

    // Position it under the scoreboard card
    const auto cardPos = _score_card.getPosition();
    const auto cardSize = _score_card.getSize();

    const float x = W * 0.5f;
    float y = cardPos.y + cardSize.y + H * 0.06f;
    y = std::min(y, H * 0.92f);

    centerText(_start_hint_shadow, x + 2.f, y + 2.f);
    centerText(_start_hint, x, y);
    // =========== Layout "Press Enter" hint [END] ===========
}

} // namespace representation
