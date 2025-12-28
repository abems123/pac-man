//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_WALL_VIEW
#define PACMANPROJECT_WALL_VIEW
#include <SFML/Graphics/RectangleShape.hpp>

#include "EntityView.h"

#include <cmath>

namespace logic {
    class Wall;
}

namespace representation {

class Wall final : public EntityView {
    /** @brief SFML shape used to draw the gate tile. */
    sf::RectangleShape _shape;

    bool _is_gate = false;

    /** @brief Texture frames for standard wall tiles. */
    std::vector<sf::IntRect> _normal_wall_frames;
    /** @brief Texture frames for edge wall tiles. */
    std::vector<sf::IntRect> _edge_wall_frames;
    /** @brief Texture frames for T-junction wall tiles. */
    std::vector<sf::IntRect> _t_kruispunt_frames;
    /** @brief Texture frames for end-cap/tip wall tiles. */
    std::vector<sf::IntRect> _tip_walls;

    /** @brief Whether the wall visuals were initialized/updated at least once. */
    bool updated = false;

public:
    /**
     * @brief Constructs a wall view observing the given model and optionally treating it as a gate.
     * @param model Model to observe; non-owning (owned by the World).
     * @param isGate Whether this wall should be rendered/handled as the ghost gate.
     */
    explicit Wall(const std::shared_ptr<logic::EntityModel>& model, bool isGate = false);

    /**
     * @brief Receives model events and updates wall visuals when needed.
     * @param event Event emitted by the observed model.
     */
    void onNotify(const logic::EventType& event) override;

    /**
     * @brief Recomputes wall appearance (shape/texture frame selection).
     * @return void
     */
    void updateWall();
    /**
     * @brief Updates wall animation/state each frame.
     * @param dt Delta time in seconds.
     */
    void update(float dt) override;

    /**
     * @brief Draws the wall to the window.
     * @param window Render target to draw to.
     */
    void render(sf::RenderWindow& window) const override;

    /**
     * @brief Default virtual destructor.
     */
    ~Wall() override = default;

};
} // namespace representation

#endif // PACMANPROJECT_WALL_VIEW
