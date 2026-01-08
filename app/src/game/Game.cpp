//
// Created by abdellah on 12/12/25.
//

#include "game/Game.h"

#include "state/MenuState.h"

#include <algorithm>
#include <stdexcept>
#include <string_view>
#include <utils/Stopwatch.h>

namespace representation {

namespace {
using namespace std::literals;

constexpr std::string_view kMusicMenu = "../../assets/soundeffects/music_menu.wav"sv;
constexpr std::string_view kMusicLevel = "../../assets/soundeffects/music_level.wav"sv;

constexpr std::string_view kSfxCoin = "../../assets/soundeffects/sfx_turn_corner.wav"sv;
constexpr std::string_view kSfxFruit = "../../assets/soundeffects/sfx_fruit.wav"sv;
constexpr std::string_view kSfxGhost = "../../assets/soundeffects/sfx_ghost_eaten.wav"sv;
constexpr std::string_view kSfxDie = "../../assets/soundeffects/sfx_pacman_die.wav"sv;
constexpr std::string_view kSfxFright = "../../assets/soundeffects/sfx_frightened_start.wav"sv;
} // namespace

Game::Game() {
    checkMapConsistent();

    _window.create(sf::VideoMode::getDesktopMode(), "Pac Man");
    _window.setVerticalSyncEnabled(true);
    _window.setFramerateLimit(0);

    // initial menu
    _stateManager.pushState(std::make_unique<MenuState>(_stateManager, *this));

    // ======== Camera Singleton initializing [START] ========
    Camera::instance().init(_window.getSize().x, _window.getSize().y);
    _windowSize = _window.getSize();
    // ======== Camera Singleton initializing [END] ========

    loadAudioAssets();

    // Default volumes
    setMasterVolume(1.0f);
    setSfxVolume(1.0f);
}

void Game::setMusicEnabled(const bool enabled) {
    _music_enabled = enabled;

    if (!_music_enabled) {
        stopMusic();
    }

    applyAudioVolumes();
}

bool Game::isMusicEnabled() const { return _music_enabled; }

void Game::setMasterVolume(float volume) {
    _master_volume = std::clamp(volume, 0.f, 1.f);
    applyAudioVolumes();
}

float Game::getMasterVolume() const { return _master_volume; }

void Game::setMusicVolume(float volume01) {
    // Keep old name used by LevelState, but treat it as MASTER volume.
    setMasterVolume(volume01);
}

float Game::getMusicVolume() const { return _master_volume; }

void Game::setSfxVolume(const float volume01) {
    _sfx_volume01 = std::clamp(volume01, 0.f, 1.f);
    applyAudioVolumes();
}

float Game::getSfxVolume() const { return _sfx_volume01; }

void Game::applyAudioVolumes() {
    // =========== Apply audio volumes [START] ===========
    const float musicVol = (_music_enabled ? _master_volume : 0.f) * 100.f;
    _music.setVolume(musicVol);

    const float sfxVol = (_master_volume * _sfx_volume01) * 100.f;

    // Keep pool sounds consistent (playing or not)
    for (auto& s : _sfx_pool) {
        s.setVolume(sfxVol);
    }
    // =========== Apply audio volumes [END] ===========
}

void Game::loadAudioAssets() {
    // =========== Load audio assets [START] ===========
    _sfx_pool.clear();
    _sfx_pool.resize(12);

    auto loadBuf = [&](Sfx id, std::string_view path) {
        sf::SoundBuffer buf;
        if (!buf.loadFromFile(std::string(path))) {
            throw std::runtime_error("Missing SFX file: " + std::string(path));
        }
        _sfx_buffers.emplace(id, std::move(buf));
    };

    loadBuf(Sfx::Coin, kSfxCoin);
    loadBuf(Sfx::Fruit, kSfxFruit);
    loadBuf(Sfx::GhostEaten, kSfxGhost);
    loadBuf(Sfx::PacmanDied, kSfxDie);
    loadBuf(Sfx::FrightenedStart, kSfxFright);
    // =========== Load audio assets [END] ===========
}

void Game::playMusic(const MusicTrack track, const bool loop) {
    // =========== Play music [START] ===========
    if (!_music_enabled) {
        stopMusic();
        return;
    }

    if (track == _active_music && _music.getStatus() == sf::SoundSource::Playing) {
        _music.setLoop(loop);
        return;
    }

    std::string_view path{};
    switch (track) {
    case MusicTrack::Menu:
        path = kMusicMenu;
        break;
    case MusicTrack::Level:
        path = kMusicLevel;
        break;
    default:
        break;
    }

    _music.stop();
    _active_music = MusicTrack::None;

    if (path.empty())
        return;

    if (!_music.openFromFile(std::string(path)))
        throw std::runtime_error("Missing music file: " + std::string(path));

    _active_music = track;

    _music.setLoop(loop);
    applyAudioVolumes();
    _music.play();
    // =========== Play music [END] ===========
}

void Game::stopMusic() {
    // =========== Stop music [START] ===========
    _music.stop();
    _active_music = MusicTrack::None;
    // =========== Stop music [END] ===========
}

void Game::playSfx(const Sfx id) {
    const auto it = _sfx_buffers.find(id);
    if (it == _sfx_buffers.end())
        return;

    // Find a free sound in the pool
    const auto freeIt = std::find_if(_sfx_pool.begin(), _sfx_pool.end(),
                                     [](const sf::Sound& s) { return s.getStatus() != sf::SoundSource::Playing; });

    sf::Sound& s = (freeIt != _sfx_pool.end()) ? *freeIt : _sfx_pool.front();

    s.setBuffer(it->second);

    // Effective SFX volume = MASTER * SFX_MULT
    const float sfxVol = (_master_volume * _sfx_volume01) * 100.f;
    s.setVolume(sfxVol);

    s.play();
}

void Game::applyResizeIfNeeded() {
    const auto sz = _window.getSize();
    if (sz == _windowSize)
        return;

    _windowSize = sz;

    // Keep pixel coordinate view consistent everywhere.
    _window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(sz.x), static_cast<float>(sz.y))));

    // One global source of truth for map rect.
    Camera::instance().init(sz.x, sz.y);

    // Make every state fix its layout immediately (even those not currently active).
    _stateManager.notifyResize(sz.x, sz.y);
}

void Game::run() {
    while (_window.isOpen()) {
        logic::Stopwatch::getInstance().tick();
        const auto dt = logic::Stopwatch::getInstance().dt();

        applyResizeIfNeeded();

        _stateManager.handleInput();
        _stateManager.update(dt);

        _window.clear(sf::Color::Black);
        _stateManager.render(_window);
        _window.display();
    }
}

void Game::checkMapConsistent() {
    auto map = ResourceManager::getMap();

    int _first_line_len = map.front().size();

    const bool inconsistent_lines_lengths =
        std::ranges::any_of(map, [_first_line_len](const auto& line) { return line.size() != _first_line_len; });

    if (inconsistent_lines_lengths) {
        throw std::runtime_error("Invalid map: some row has different number of elements than other rows");
    }
}

} // namespace representation