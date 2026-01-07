//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_LEVELSTATE_H
#define PACMANPROJECT_LEVELSTATE_H

#include <array>
#include <memory>
#include <vector>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "State.h"
#include "world/World.h"

namespace representation {

enum class Font;
class EntityView;

/**
 * @brief Gameplay state that runs a level (world update + entity rendering + HUD).
 *
 * This state owns a logic::World instance and a set of EntityView objects that render it.
 * It also manages the HUD (score, lives, bottom-right dock widgets).
 */
class LevelState final : public State {
private:
    // ===================== Audio-driven event cache (SFX triggers) =====================

    /** @brief Previous frame's remaining coin count (used to detect coin pickups). */
    int _prev_coins_left{-1};

    /** @brief Previous frame's remaining fruit count (used to detect fruit pickups). */
    int _prev_fruits_left{-1};

    /** @brief Previous frame's remaining lives count (used to detect life loss). */
    int _prev_lives_left{-1};

    /** @brief Previous frame's total eaten ghosts (used to detect ghost eaten event). */
    int _prev_ghosts_eaten_total{-1};

    /** @brief Previous frame's frightened time left (used to detect frightened start). */
    float _prev_frightened_left{0.f};

    // ===================== Bottom HUD dock widgets =====================

    /** @brief Background container for the bottom-right dock UI. */
    sf::RectangleShape _hud_dock{};

    /** @brief Switch knob used for toggling between WASD and arrow controls. */
    sf::CircleShape _controls_knob{};

    /** @brief Text that shows the current input scheme ("WASD" or "ARROWS"). */
    sf::Text _controls_value_text{};

    /**
     * @brief Initializes the bottom-right dock UI elements (texts/shapes defaults).
     */
    void initBottomRightHud();

    /**
     * @brief Recomputes bottom-right dock positions based on the current window size.
     * @param window Render window used for sizing/layout.
     */
    void layoutBottomRightHud(const sf::RenderWindow& window);

    /**
     * @brief Updates the volume slider fill and knob position from the current volume value.
     */
    void updateVolumeVisuals();

    /**
     * @brief Updates the controls switch knob position from the current input scheme.
     */
    void updateControlsVisuals();

    /**
     * @brief Checks if a mouse position is inside a rectangle shape.
     * @param shape Shape to test against.
     * @param mousePx Mouse position in window pixel coordinates.
     * @return True if the mouse is inside the shape bounds.
     */
    bool isMouseOver(const sf::RectangleShape& shape, sf::Vector2i mousePx) const;

    /**
     * @brief Sets the "master volume" value based on a mouse X position over the slider.
     *
     * NOTE: This slider controls MASTER volume (music + sfx). Music is still gated by
     * Game::isMusicEnabled().
     *
     * @param mouseX Mouse X position in window pixel coordinates.
     */
    void setMusicVolumeFromMouseX(int mouseX);

    // ===================== Input scheme + volume UI state =====================

    /** @brief True when movement input uses arrow keys instead of WASD. */
    bool _use_arrow_controls{false};

    /**
     * @brief Volume slider value in range [0..1].
     *
     * This mirrors Game::getMasterVolume() and is updated when dragging.
     */
    float _music_volume{1.0f};

    /** @brief True while the user is dragging the volume slider. */
    bool _dragging_volume{false};

    /** @brief Slider background bar for the volume UI. */
    sf::RectangleShape _volume_bg{};

    /** @brief Filled portion of the volume slider. */
    sf::RectangleShape _volume_fill{};

    /** @brief Slider knob for the volume UI. */
    sf::RectangleShape _volume_knob{};

    /** @brief "VOL" label shown next to the volume slider. */
    sf::Text _volume_text{};

    /** @brief Switch track used for toggling the input scheme. */
    sf::RectangleShape _controls_bg{};

    /** @brief "KEYS" label shown next to the input switch. */
    sf::Text _controls_text{};

    // ===================== Lives HUD =====================

    /** @brief Three life icons shown at the bottom (alive or gray). */
    std::array<sf::Sprite, 3> _life_icons{};

    /** @brief Texture rect for an alive life icon (Pac-Man). */
    sf::IntRect _life_alive_rect{};

    /** @brief Texture rect for a lost life icon (gray frame). */
    sf::IntRect _life_dead_rect{};

    // ===================== World + views =====================

    /** @brief Owning gameplay world instance for the current level. */
    std::unique_ptr<logic::World> _world{};

    /** @brief Views for entities created in this level. */
    std::vector<std::shared_ptr<EntityView>> _views{};

    /** @brief Factory used to create model/view pairs for the current level. */
    std::shared_ptr<logic::AbstractFactory> _factory{};

    // ===================== Top HUD texts =====================

    /** @brief Score text displayed in the top HUD. */
    sf::Text score{};

    /** @brief True once we already pushed an end screen for this level. */
    bool _end_screen_pushed{false};

    /** @brief Current level number. */
    int _level{1};

public:
    /**
     * @brief Creates a new gameplay state for a level.
     * @param manager State stack manager.
     * @param game Game context (window/resources).
     * @param level Level number.
     * @param carryScore Score carried from previous level.
     * @param carryLives Lives carried from previous level.
     */
    LevelState(StateManager& manager, Game& game, int level, int carryScore = 0, int carryLives = 3);

    /**
     * @brief Recomputes HUD text sizes and positions based on the current window size.
     */
    void updateHudLayout();
    void updateBottomRightHudTextSizes();

    /**
     * @brief Reads user input and converts it to game actions (movement, pause, HUD toggles).
     */
    void handleInput() override;

    /**
     * @brief Updates the world and views and checks win/lose conditions.
     * @param dt Delta time in seconds.
     */
    void update(float dt) override;

    /**
     * @brief Draws the world and HUD.
     * @param window Render window to draw into.
     */
    void render(sf::RenderWindow& window) override;

private:
    /** @brief Text that shows the current level number in the top HUD. */
    sf::Text _level_text{};

    /**
     * @brief Positions top HUD texts relative to the map viewport.
     */
    void layoutTopHud();

    /**
     * @brief Refreshes HUD strings (score + level) for the current frame.
     */
    void updateHud();

    /**
     * @brief Initializes all HUD elements (texts, lives icons, bottom-right dock).
     */
    void initHud();

    /**
     * @brief Configures an SFML text element.
     * @param text Text object to configure.
     * @param font Font enum used by ResourceManager.
     * @param charSize Character size in pixels.
     * @param pos Screen position in pixels.
     * @param color Fill color.
     */
    void initText(sf::Text& text, Font font, unsigned int charSize, sf::Vector2f pos, sf::Color color);

    /**
     * @brief Builds a single spritesheet rect using row/col indices.
     * @param row Spritesheet row index.
     * @param col Spritesheet column index.
     * @return Rectangle for that frame in the spritesheet.
     */
    sf::IntRect singleFrameRect(int row, int col);

    /**
     * @brief Initializes life icon sprites and their alive/dead texture rectangles.
     */
    void initLivesHud();

    /**
     * @brief Draws HUD elements on top of the world.
     * @param window Render window to draw into.
     */
    void renderHud(sf::RenderWindow& window);

    /**
     * @brief Lays out life icons in the bottom HUD band.
     * @param window Render window used for sizing.
     */
    void layoutLivesHud(const sf::RenderWindow& window);

    /**
     * @brief Centers a text vertically in the top HUD band while keeping its X position.
     * @param text Text object to re-origin and reposition.
     */
    void centerTopHudText(sf::Text& text) const;

    /**
     * @brief Handles window resize and recomputes all HUD layouts.
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     */
    void onResize(unsigned width, unsigned height) override;
};

} // namespace representation

#endif // PACMANPROJECT_LEVELSTATE_H
