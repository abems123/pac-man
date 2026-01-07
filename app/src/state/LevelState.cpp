//
// Created by abdellah on 12/12/2025.
//

#include "../../include/state/LevelState.h"

#include <SFML/Window/Event.hpp>

#include <algorithm>
#include <cmath>

#include "game/Game.h"
#include "score/Score.h"
#include "state/PausedState.h"
#include "view/EntityView.h"

#include <events/EventType.h>
#include <factory/ConcreteFactory.h>
#include <state/GameOverState.h>
#include <state/VictoryState.h>
#include <utils/AnimationManager.h>
#include <utils/ResourceManager.h>

namespace representation {

LevelState::LevelState(StateManager& manager, Game& game, const int level, int carryScore, int carryLives)
    : State(manager, game), _factory(std::make_shared<ConcreteFactory>(_views)), _level(level) {
    _world = std::make_unique<logic::World>(_factory, _level, carryScore, carryLives);
    initHud();

    // Music respects Game::isMusicEnabled() internally.
    _game.playMusic(Game::MusicTrack::Level, true);

    _prev_coins_left = _world->getCoinsLeft();
    _prev_fruits_left = _world->getFruitsLeft();
    _prev_lives_left = _world->getLivesLeft();
    _prev_ghosts_eaten_total = _world->getGhostsEatenTotal();
    _prev_frightened_left = _world->getFrightenedLeft();
}

bool LevelState::isMouseOver(const sf::RectangleShape& shape, sf::Vector2i mousePx) const {
    return shape.getGlobalBounds().contains(static_cast<float>(mousePx.x), static_cast<float>(mousePx.y));
}

void LevelState::updateVolumeVisuals() {
    // =========== Update volume slider visuals [START] ===========
    _music_volume = std::clamp(_music_volume, 0.f, 1.f);

    const sf::Vector2f pos = _volume_bg.getPosition();
    const sf::Vector2f size = _volume_bg.getSize();

    _volume_fill.setPosition(pos);
    _volume_fill.setSize({size.x * _music_volume, size.y});

    const float knobW = _volume_knob.getSize().x;
    const float knobH = _volume_knob.getSize().y;

    const float knobX = pos.x + size.x * _music_volume - knobW * 0.5f;
    const float knobY = pos.y + (size.y - knobH) * 0.5f;

    _volume_knob.setPosition(knobX, knobY);
    // =========== Update volume slider visuals [END] ===========
}

void LevelState::updateControlsVisuals() {
    // =========== Update controls switch visuals [START] ===========
    const sf::Vector2f trackPos = _controls_bg.getPosition();
    const sf::Vector2f trackSize = _controls_bg.getSize();

    const float r = _controls_knob.getRadius();
    const float y = trackPos.y + (trackSize.y - 2.f * r) * 0.5f;

    const float leftX = trackPos.x + r;
    const float rightX = trackPos.x + trackSize.x - r;

    _controls_knob.setPosition((_use_arrow_controls ? rightX : leftX) - r, y);
    // =========== Update controls switch visuals [END] ===========
}

void LevelState::setMusicVolumeFromMouseX(int mouseX) {
    // =========== Convert mouse X to [0..1] volume [START] ===========
    const float left = _volume_bg.getPosition().x;
    const float width = _volume_bg.getSize().x;

    if (width <= 0.f)
        return;

    const float t = (static_cast<float>(mouseX) - left) / width;
    _music_volume = std::clamp(t, 0.f, 1.f);

    // This sets MASTER volume (music + SFX). If music is disabled, only SFX changes.
    _game.setMusicVolume(_music_volume);

    updateVolumeVisuals();
    // =========== Convert mouse X to [0..1] volume [END] ===========
}

void LevelState::initBottomRightHud() {
    // =========== Initialize bottom-right dock UI [START] ===========
    const auto win = _game.getWindow().getSize();
    const float bottomBandH = static_cast<float>(win.y) * 0.10f;

    const unsigned charSize = static_cast<unsigned>(std::clamp(bottomBandH * 0.28f, 12.f, 22.f));

    initText(_volume_text, Font::SuperMeatBall, charSize, {0.f, 0.f}, sf::Color::White);
    initText(_controls_text, Font::SuperMeatBall, charSize, {0.f, 0.f}, sf::Color::White);
    initText(_controls_value_text, Font::SuperMeatBall, charSize, {0.f, 0.f}, sf::Color::White);

    _volume_text.setString("VOL");
    _controls_text.setString("KEYS");
    _controls_value_text.setString(_use_arrow_controls ? "ARROWS" : "WASD");

    _hud_dock.setFillColor(sf::Color(20, 20, 20, 170));

    _volume_bg.setFillColor(sf::Color(55, 55, 55, 230));
    _volume_fill.setFillColor(sf::Color(200, 200, 200, 245));
    _volume_knob.setFillColor(sf::Color::White);

    _controls_bg.setFillColor(sf::Color(55, 55, 55, 230));
    _controls_knob.setFillColor(sf::Color::White);

    // Initialize slider from MASTER volume.
    _music_volume = std::clamp(_game.getMasterVolume(), 0.f, 1.f);

    layoutBottomRightHud(_game.getWindow());
    // =========== Initialize bottom-right dock UI [END] ===========
}

void LevelState::layoutBottomRightHud(const sf::RenderWindow& window) {
    // =========== Layout compact bottom-right dock (VOL + slider + KEYS + state + switch) [START] ===========
    const auto win = window.getSize();
    const auto map = Camera::instance().getMapRectPx();

    const float bottomBandTop = static_cast<float>(win.y) * 0.90f;
    const float bottomBandH = static_cast<float>(win.y) * 0.10f;

    const float dockH = std::clamp(bottomBandH * 0.62f, 30.f, 44.f);

    const float pad = std::clamp(map.width * 0.010f, 8.f, 14.f);
    const float gap = std::clamp(map.width * 0.015f, 12.f, 20.f);
    const float smallGap = std::clamp(map.width * 0.008f, 8.f, 12.f);

    const float musicShift = std::clamp(map.width * 0.02f, 12.f, 28.f);

    const float sliderW = std::clamp(map.width * 0.18f, 170.f, 280.f);
    const float sliderH = std::clamp(dockH * 0.22f, 8.f, 12.f);

    const float switchW = std::clamp(map.width * 0.060f, 55.f, 75.f);
    const float switchH = std::clamp(dockH * 0.48f, 18.f, 26.f);

    const float volW = _volume_text.getLocalBounds().width;
    const float keysW = _controls_text.getLocalBounds().width;
    const float valW = _controls_value_text.getLocalBounds().width;

    const float dockW = pad + volW + pad + sliderW + gap + keysW + smallGap + valW + pad + switchW + pad;

    const float marginX = map.width * 0.02f;
    const float dockX = (map.left + map.width - marginX) - dockW;
    const float dockY = bottomBandTop + (bottomBandH - dockH) * 0.5f;

    _hud_dock.setPosition({dockX, dockY});
    _hud_dock.setSize({dockW, dockH});

    {
        const auto b = _volume_text.getLocalBounds();
        _volume_text.setOrigin(b.left, b.top + b.height * 0.5f);
        _volume_text.setPosition(dockX + pad - musicShift, dockY + dockH * 0.5f);
    }

    const float sliderX = _volume_text.getPosition().x + volW + pad;
    const float sliderY = dockY + (dockH - sliderH) * 0.5f;

    _volume_bg.setPosition({sliderX, sliderY});
    _volume_bg.setSize({sliderW, sliderH});

    const float knobSize = std::clamp(sliderH * 2.4f, 14.f, 20.f);
    _volume_knob.setSize({knobSize, knobSize});

    const float keysX = sliderX + sliderW + gap;
    {
        const auto b = _controls_text.getLocalBounds();
        _controls_text.setOrigin(b.left, b.top + b.height * 0.5f);
        _controls_text.setPosition(keysX, dockY + dockH * 0.5f);
    }

    const float valX = keysX + keysW + smallGap;
    {
        const auto b = _controls_value_text.getLocalBounds();
        _controls_value_text.setOrigin(b.left, b.top + b.height * 0.5f);
        _controls_value_text.setPosition(valX, dockY + dockH * 0.5f);
    }

    const float switchX = valX + valW + pad;
    const float switchY = dockY + (dockH - switchH) * 0.5f;

    _controls_bg.setPosition({switchX, switchY});
    _controls_bg.setSize({switchW, switchH});

    const float r = (switchH * 0.5f) - 2.f;
    _controls_knob.setRadius(std::max(6.f, r));

    updateVolumeVisuals();
    updateControlsVisuals();
    // =========== Layout compact bottom-right dock (VOL + slider + KEYS + state + switch) [END] ===========
}

void LevelState::initHud() {
    // =========== Initialize HUD based on window size [START] ===========
    const auto size = _game.getWindow().getSize();
    const unsigned int hudSize = static_cast<unsigned int>(size.y * 0.05f);

    initText(score, representation::Font::SuperMeatBall, hudSize, {0.f, 0.f}, sf::Color::White);
    score.setString("Score: 0");

    // =========== Initialize level text [START] ===========
    initText(_level_text, Font::SuperMeatBall, hudSize, {0.f, 0.f}, sf::Color::White);
    _level_text.setString("Level: " + std::to_string(_level));
    // =========== Initialize level text [END] ===========

    initLivesHud();
    updateHudLayout();
    layoutTopHud();
    layoutLivesHud(_game.getWindow());
    initBottomRightHud();
    // =========== Initialize HUD based on window size [END] ===========
}

void LevelState::initText(sf::Text& text, Font font, unsigned int charSize, sf::Vector2f pos, sf::Color color) {
    // =========== Setup an SFML text element [START] ===========
    text.setFont(ResourceManager::instance().getFont(font));
    text.setCharacterSize(charSize);
    text.setPosition(pos);
    text.setFillColor(color);
    // =========== Setup an SFML text element [END] ===========
}

sf::IntRect LevelState::singleFrameRect(const int row, const int col) {
    std::vector<sf::IntRect> frames;
    AnimationManager::setFrames(col, row, 1, frames);
    return frames.front();
}

void LevelState::initLivesHud() {
    // =========== Setup lives label + icon sprites [START] ===========
    _life_alive_rect = singleFrameRect(1, 17);
    _life_dead_rect = singleFrameRect(1, 18);

    const sf::Texture& sheet = ResourceManager::instance().getTexture();
    for (auto& s : _life_icons) {
        s.setTexture(sheet);
        s.setTextureRect(_life_alive_rect);
    }
    // =========== Setup lives label + icon sprites [END] ===========
}

void LevelState::handleInput() {
    using sf::Keyboard;

    // =========== Movement input (scheme toggled) [START] ===========
    if (_use_arrow_controls) {
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            _world->changePacmanDirection(logic::Direction::Right);
        } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
            _world->changePacmanDirection(logic::Direction::Left);
        } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
            _world->changePacmanDirection(logic::Direction::Up);
        } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
            _world->changePacmanDirection(logic::Direction::Down);
        }
    } else {
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            _world->changePacmanDirection(logic::Direction::Right);
        } else if (Keyboard::isKeyPressed(Keyboard::A)) {
            _world->changePacmanDirection(logic::Direction::Left);
        } else if (Keyboard::isKeyPressed(Keyboard::W)) {
            _world->changePacmanDirection(logic::Direction::Up);
        } else if (Keyboard::isKeyPressed(Keyboard::S)) {
            _world->changePacmanDirection(logic::Direction::Down);
        }
    }
    // =========== Movement input (scheme toggled) [END] ===========

    sf::Event event{};
    auto& window = _game.getWindow();
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            _state_manager.pushState(std::make_unique<PausedState>(_state_manager, _game));
            return;
        }

        // =========== Toggle control scheme (TAB) [START] ===========
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
            _use_arrow_controls = !_use_arrow_controls;
            _controls_value_text.setString(_use_arrow_controls ? "ARROWS" : "WASD");
            layoutBottomRightHud(window);
        }
        // =========== Toggle control scheme (TAB) [END] ===========

        // =========== Volume slider mouse input [START] ===========
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2i mousePx(event.mouseButton.x, event.mouseButton.y);

            if (isMouseOver(_controls_bg, mousePx)) {
                _use_arrow_controls = !_use_arrow_controls;
                _controls_value_text.setString(_use_arrow_controls ? "ARROWS" : "WASD");
                layoutBottomRightHud(window);
            } else if (isMouseOver(_volume_bg, mousePx)) {
                _dragging_volume = true;
                setMusicVolumeFromMouseX(mousePx.x);
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            _dragging_volume = false;
        } else if (event.type == sf::Event::MouseMoved) {
            if (_dragging_volume)
                setMusicVolumeFromMouseX(event.mouseMove.x);
        }
        // =========== Volume slider mouse input [END] ===========

        else if (event.type == sf::Event::Resized) {
            // =========== Fix SFML pixel coordinate system [START] ===========
            window.setView(sf::View(
                sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
            // =========== Fix SFML pixel coordinate system [END] ===========

            Camera::instance().init(event.size.width, event.size.height);
            onResize(event.size.width, event.size.height);

            for (auto& v : _views)
                if (v)
                    v->updatePosition();

            _state_manager.pushState(std::make_unique<PausedState>(_state_manager, _game));
        }
    }
}

void LevelState::updateHudLayout() {
    const auto win = _game.getWindow().getSize();
    const float topHudH = static_cast<float>(win.y) * 0.10f;
    const unsigned charSize = static_cast<unsigned>(std::clamp(topHudH * 0.55f, 12.f, 48.f));

    score.setCharacterSize(charSize);
    _level_text.setCharacterSize(charSize);

    centerTopHudText(score);
    centerTopHudText(_level_text);
}

void LevelState::updateBottomRightHudTextSizes() {
    // =========== Update bottom-right HUD text sizes (resize) [START] ===========
    const auto win = _game.getWindow().getSize();
    const float bottomBandH = static_cast<float>(win.y) * 0.10f;

    const unsigned charSize = static_cast<unsigned>(std::clamp(bottomBandH * 0.28f, 12.f, 22.f));

    _volume_text.setCharacterSize(charSize);
    _controls_text.setCharacterSize(charSize);
    _controls_value_text.setCharacterSize(charSize);
    // =========== Update bottom-right HUD text sizes (resize) [END] ===========
}


void LevelState::update(const float dt) {
    _world->update(dt);

    const int coins_left = _world->getCoinsLeft();
    const int fruits_left = _world->getFruitsLeft();
    const int lives_left = _world->getLivesLeft();
    const int ghosts_eaten_total = _world->getGhostsEatenTotal();
    const float frightened_left = _world->getFrightenedLeft();

    if (_prev_coins_left >= 0 && coins_left < _prev_coins_left)
        _game.playSfx(Game::Sfx::Coin);

    if (_prev_fruits_left >= 0 && fruits_left < _prev_fruits_left)
        _game.playSfx(Game::Sfx::Fruit);

    if (_prev_lives_left >= 0 && lives_left < _prev_lives_left)
        _game.playSfx(Game::Sfx::PacmanDied);

    if (_prev_ghosts_eaten_total >= 0 && ghosts_eaten_total > _prev_ghosts_eaten_total)
        _game.playSfx(Game::Sfx::GhostEaten);

    if (_prev_frightened_left <= 0.f && frightened_left > 0.f)
        _game.playSfx(Game::Sfx::FrightenedStart);

    _prev_coins_left = coins_left;
    _prev_fruits_left = fruits_left;
    _prev_lives_left = lives_left;
    _prev_ghosts_eaten_total = ghosts_eaten_total;
    _prev_frightened_left = frightened_left;

    updateHud();

    for (auto& v : _views)
        if (v)
            v->update(dt);

    std::erase_if(_views, [](const std::shared_ptr<EntityView>& v) { return !v || v->modelExpired(); });

    // =========== Push end screen after cleanup so last coin isn't shown [START] ===========
    if (!_end_screen_pushed) {
        if (_world->getCoinsLeft() <= 0 && _world->getFruitsLeft() <= 0) {
            _world->getScore().onNotify(logic::EventType::LevelCleared);
            const int finalScore = _world->getScore().getScore();
            const int livesLeft = _world->getLivesLeft();
            _state_manager.pushState(
                std::make_unique<VictoryState>(_state_manager, _game, _level, finalScore, livesLeft));
            _end_screen_pushed = true;
        } else if (_world->getLivesLeft() <= 0) {
            const int finalScore = _world->getScore().getScore();
            _state_manager.pushState(std::make_unique<GameOverState>(_state_manager, _game, _level, finalScore));
            _end_screen_pushed = true;
        }
    }
    // =========== Push end screen after cleanup so last coin isn't shown [END] ===========
}

void LevelState::updateHud() {
    score.setString("Score: " + std::to_string(_world->getScore().getScore()));

    const int lives = std::clamp(_world->getLivesLeft(), 0, 3);
    for (int i = 0; i < 3; ++i)
        _life_icons[i].setTextureRect((i < lives) ? _life_alive_rect : _life_dead_rect);

    centerTopHudText(score);
}

void LevelState::layoutLivesHud(const sf::RenderWindow& window) {
    // =========== Position life icons in bottom 10% band [START] ===========
    const auto win = window.getSize();
    const auto map = Camera::instance().getMapRectPx();

    const float bottomBandTop = static_cast<float>(win.y) * 0.90f;
    const float bottomBandH = static_cast<float>(win.y) * 0.10f;

    const float iconTargetH = bottomBandH * 0.5f;
    const float scale = iconTargetH / static_cast<float>(_life_alive_rect.height);

    const float iconH = static_cast<float>(_life_alive_rect.height) * scale;
    const float iconW = static_cast<float>(_life_alive_rect.width) * scale;

    const float y = bottomBandTop + (bottomBandH - iconH) * 0.5f;

    float x = map.left + map.width * 0.02f;
    const float spacing = map.width * 0.015f;

    for (auto& s : _life_icons) {
        s.setScale(scale, scale);
        s.setPosition(x, y);
        x += iconW + spacing;
    }
    // =========== Position life icons in bottom 10% band [END] ===========
}

void LevelState::layoutTopHud() {
    const auto map = Camera::instance().getMapRectPx();

    score.setPosition(map.left + map.width * 0.02f, 0.f);
    _level_text.setPosition(map.left + map.width * 0.80f, 0.f);

    centerTopHudText(score);
    centerTopHudText(_level_text);
}

void LevelState::render(sf::RenderWindow& window) {
    // =========== Render world in fixed layers [START] ===========
    for (const auto& v : _views) {
        if (!v)
            continue;
        if (v->getModelType() == logic::EntityType::Wall)
            v->render(window);
    }

    for (const auto& v : _views) {
        if (!v)
            continue;
        const auto t = v->getModelType();
        if (t == logic::EntityType::Coin || t == logic::EntityType::Fruit)
            v->render(window);
    }

    for (const auto& v : _views) {
        if (!v)
            continue;
        const auto t = v->getModelType();
        if (t == logic::EntityType::Pacman || t == logic::EntityType::FollowerGhost ||
            t == logic::EntityType::LockedGhost || t == logic::EntityType::LookaheadChaseGhost)
            v->render(window);
    }
    // =========== Render world in fixed layers [END] ===========

    renderHud(window);
}

void LevelState::renderHud(sf::RenderWindow& window) {
    // =========== Draw HUD elements [START] ===========
    window.draw(score);
    window.draw(_level_text);

    for (const auto& s : _life_icons)
        window.draw(s);

    window.draw(_hud_dock);

    window.draw(_volume_text);
    window.draw(_volume_bg);
    window.draw(_volume_fill);
    window.draw(_volume_knob);

    window.draw(_controls_text);
    window.draw(_controls_value_text);
    window.draw(_controls_bg);
    window.draw(_controls_knob);
    // =========== Draw HUD elements [END] ===========
}

void LevelState::centerTopHudText(sf::Text& text) const {
    // =========== Center a text vertically in the top HUD band [START] ===========
    const auto win = _game.getWindow().getSize();
    const float topBandH = static_cast<float>(win.y) * 0.10f;
    const float yCenter = topBandH * 0.5f;

    const auto b = text.getLocalBounds();
    const float x = text.getPosition().x;

    text.setOrigin(b.left, b.top + b.height * 0.5f);
    text.setPosition(x, yCenter);
    // =========== Center a text vertically in the top HUD band [END] ===========
}

void LevelState::onResize(const unsigned width, const unsigned height) {
    updateHudLayout();
    layoutTopHud();
    layoutLivesHud(_game.getWindow());

    updateBottomRightHudTextSizes();
    layoutBottomRightHud(_game.getWindow());

    for (auto& v : _views)
        if (v)
            v->updatePosition();
}

} // namespace representation