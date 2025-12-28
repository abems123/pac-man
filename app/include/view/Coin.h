//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_COIN_VIEW_H
#define PACMANPROJECT_COIN_VIEW_H
#include "EntityView.h"

namespace logic {
class Coin;
}

namespace representation {

/**
 * @brief View for a coin collectable.
 *
 * Draws a coin on screen and follows the model position using the Camera.
 * Can react to model events (e.g., when the coin gets eaten).
 */
class Coin final : public EntityView {
    /** Simple circle used to render the coin. */
    sf::CircleShape _shape;

    bool is_drawn = false;

public:
    /**
     * @brief Create a coin view bound to a coin model.
     * @param model Shared pointer to the logic coin model.
     */
    explicit Coin(const std::shared_ptr<logic::Coin>& model);

    /**
     * @brief Update the coin's screen position and size using the current camera tile size.
     *
     * Called after camera changes (resize) or when the model moved.
     */
    void updatePosition() override;

    /**
     * @brief React to events (e.g. coin eaten).
     * @param event Event that was emitted by the model.
     */
    void onNotify(const logic::EventType& event) override {}

    /**
     * @brief Draw the coin to the window.
     * @param window Render target.
     */
    void render(sf::RenderWindow& window) const override;
};
} // namespace representation

#endif // PACMANPROJECT_COIN_VIEW_H
