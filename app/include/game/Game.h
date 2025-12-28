//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_GAME_H
#define PACMANPROJECT_GAME_H

#include "camera/Camera.h"
#include "state/StateManager.h"
#include "utils/ResourceManager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace representation {

/**
 * @brief Top-level application controller for the SFML version of Pac-Man.
 *
 * Game owns the main SFML window and drives the main loop. It also coordinates
 * the representation layer infrastructure:
 * - creating/configuring the window
 * - initializing the Camera singleton
 * - managing the active screen through StateManager (menu/level/overlays)
 * - detecting window resize and propagating it to the camera + states
 * - owning and applying global audio settings (music enabled + master volume)
 */
class Game {
private:
    /**
     * @brief Main SFML render window for the application.
     *
     * The game loop runs while this window is open. All rendering is performed onto this
     * window, and input events are polled from it by the active state.
     */
    sf::RenderWindow _window{};

    /**
     * @brief Stack-based state controller (Menu, Level, Pause, Victory, GameOver, ...).
     *
     * The state manager owns the active state stack and forwards input/update/render
     * calls to the appropriate state(s).
     */
    StateManager _stateManager{};

    /**
     * @brief Cached window size used to detect resize events between frames.
     *
     * applyResizeIfNeeded() compares the current SFML window size to this cached value.
     * When the size changes, the game updates SFML's view, updates the Camera singleton,
     * and notifies all states so UI/layout can be rebuilt.
     */
    sf::Vector2<unsigned> _windowSize{0u, 0u};

    /**
     * @brief List of currently playing SFX sounds (optional approach).
     *
     * If you decide to keep "active sound instances" to update their volume dynamically
     * while they are playing, store them here. If you use a pool instead, you can keep
     * this empty and just update the pool.
     */
    std::vector<sf::Sound> _active_sounds{};

    /**
     * @brief Hash helper for strongly-typed enums used as unordered_map keys.
     */
    struct EnumHash {
        /**
         * @brief Hash operator for enum values.
         * @tparam T Enum type.
         * @param v Enum value.
         * @return Hash as size_t.
         */
        template <typename T>
        std::size_t operator()(T v) const noexcept {
            return static_cast<std::size_t>(v);
        }
    };

    /**
     * @brief Loads sound buffers and initializes the SFX playback pool.
     *
     * This should be called once during Game construction.
     */
    void loadAudioAssets();

public:
    /**
     * @brief Construct the game and initialize core rendering infrastructure.
     *
     * This creates the window, initializes Camera, pushes the initial MenuState,
     * and loads all audio assets.
     */
    Game();

    /**
     * @brief Available background music tracks.
     */
    enum class MusicTrack {
        /** No track selected. */
        None,
        /** Menu background track. */
        Menu,
        /** Level gameplay track. */
        Level
    };

    /**
     * @brief Available sound effects (SFX) identifiers.
     */
    enum class Sfx {
        /** Coin pickup sound. */
        Coin,
        /** Fruit pickup sound. */
        Fruit,
        /** Ghost eaten sound. */
        GhostEaten,
        /** Pac-Man death sound. */
        PacmanDied,
        /** Start of frightened mode sound. */
        FrightenedStart
    };

    /**
     * @brief Start playing a background music track (respects global music enabled flag).
     * @param track Which track to play.
     * @param loop True to loop the track.
     */
    void playMusic(MusicTrack track, bool loop);

    /**
     * @brief Stop any currently playing background music.
     */
    void stopMusic();

    /**
     * @brief Set the "music volume" slider value used by the UI.
     *
     * IMPORTANT: In this project, this is treated as MASTER volume:
     * - If music is enabled -> affects music volume.
     * - Always affects sound effects volume.
     *
     * @param volume01 Volume in range [0..1].
     */
    void setMusicVolume(float volume01);

    /**
     * @brief Get the current music volume value in range [0..1].
     * @return Current music volume value.
     */
    [[nodiscard]] float getMusicVolume() const;

    /**
     * @brief Play a sound effect by ID (respects current master volume).
     * @param id Sound effect identifier.
     */
    void playSfx(Sfx id);

    /**
     * @brief Set SFX relative volume in range [0..1].
     *
     * This is multiplied by master volume to compute final SFX volume.
     *
     * @param volume01 SFX volume in range [0..1].
     */
    void setSfxVolume(float volume01);

    /**
     * @brief Get SFX relative volume in range [0..1].
     * @return Current SFX volume value.
     */
    [[nodiscard]] float getSfxVolume() const;

    /**
     * @brief Apply resize handling if the window size has changed since the last frame.
     *
     * This fixes SFML's view, updates Camera singleton, and notifies states.
     */
    void applyResizeIfNeeded();

    /**
     * @brief Run the main loop until the window is closed.
     */
    void run();

    /**
     * @brief Validate that the loaded map is consistent before starting.
     *
     * This typically checks row lengths etc. Throws if invalid.
     */
    static void checkMapConsistent();

    /**
     * @brief Access the underlying SFML window.
     * @return Reference to the window owned by Game.
     */
    sf::RenderWindow& getWindow() { return _window; }

    /**
     * @brief Access the ResourceManager singleton.
     * @return ResourceManager instance reference.
     */
    const ResourceManager& getResources() const { return ResourceManager::instance(); }

    // ===================== Global audio controls =====================

    /**
     * @brief Enable or disable background music globally.
     *
     * When disabled, any currently playing music is stopped and music volume becomes 0.
     * SFX is still allowed.
     *
     * @param enabled True to allow music playback, false to mute/stop it.
     */
    void setMusicEnabled(bool enabled);

    /**
     * @brief Check if background music is globally enabled.
     * @return True if music is enabled, false otherwise.
     */
    [[nodiscard]] bool isMusicEnabled() const;

    /**
     * @brief Set the global master volume in range [0..1].
     *
     * This master volume affects:
     * - Music volume if music is enabled.
     * - Sound effect volume always.
     *
     * @param volume Volume in [0..1].
     */
    void setMasterVolume(float volume);

    /**
     * @brief Get the global master volume in range [0..1].
     * @return Current master volume.
     */
    [[nodiscard]] float getMasterVolume() const;

private:
    /**
     * @brief True if background music is allowed to play.
     */
    bool _music_enabled{true};

    /**
     * @brief Global master volume in range [0..1].
     *
     * Applied to:
     * - music (only if _music_enabled)
     * - sfx (always)
     */
    float _master_volume{1.0f};

    /**
     * @brief Apply current audio settings to underlying SFML audio objects.
     *
     * This updates:
     * - _music volume (0 if music disabled)
     * - all currently playing SFX (via _active_sounds or pool objects)
     */
    void applyAudioVolumes();

    /**
     * @brief SFML music stream used for background music.
     */
    sf::Music _music{};

    /**
     * @brief Currently active music track (for avoiding unnecessary reloads).
     */
    MusicTrack _active_music{MusicTrack::None};

    /**
     * @brief Cached UI "music volume" value in range [0..1].
     *
     * NOTE: In this project, the slider acts as master volume.
     */
    float _music_volume01{1.0f};

    /**
     * @brief Cached relative SFX volume in range [0..1].
     *
     * Final SFX volume = _master_volume * _sfx_volume01.
     */
    float _sfx_volume01{1.0f};

    /**
     * @brief Loaded SFX buffers indexed by Sfx enum.
     */
    std::unordered_map<Sfx, sf::SoundBuffer, EnumHash> _sfx_buffers{};

    /**
     * @brief Pool of reusable sf::Sound instances to allow overlapping sound playback.
     *
     * playSfx() will take a free sound, assign its buffer, set volume, and play it.
     */
    std::vector<sf::Sound> _sfx_pool{};
};

} // namespace representation

#endif // PACMANPROJECT_GAME_H
