//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_PAUSEDSTATE_H
#define PACMANPROJECT_PAUSEDSTATE_H
#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

namespace representation {
class PausedState final : public State {
public:
    /** Builds the pause menu overlay. */
    PausedState(StateManager& manager, Game& game);

    void handleInput() override;
    void update(float dt) override {}
    void render(sf::RenderWindow& window) override;

private:
    // Panel/card look
    sf::RectangleShape _panel_shadow;
    sf::RectangleShape _panel;

    // Optional extra text lines
    sf::Text _subtitle;
    sf::Text _hint;

    // Background “pill” behind each option (only visible for selected)
    std::vector<sf::RectangleShape> _option_boxes;

    /** Dark overlay behind the pause texts. */
    sf::RectangleShape _overlay;

    /** Title text ("Paused"). */
    sf::Text _title;

    /** Menu option texts (Resume / Settings / Exit). */
    std::vector<sf::Text> _options;

    /** Currently selected option index. */
    int _selected{0};

    /** Creates and positions title + options. */
    void buildMenu();

    /** Updates colors to reflect selection. */
    void updateSelectionVisuals();

    /** Activates the currently selected option. */
    void activateSelected() const;

    /** @brief Rebuild pause menu layout after a window resize. */
    void onResize(unsigned width, unsigned height) override;

    bool isOverlay() const override { return true; }
};
} // namespace representation

#endif // PACMANPROJECT_PAUSEDSTATE_H