//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_FRUIT_VIEW_H
#define PACMANPROJECT_FRUIT_VIEW_H
#include "EntityView.h"

namespace representation {
class Fruit final : public EntityView {
public:
    /**
     * @brief Constructs a fruit view observing the given model.
     * @param model Model to observe; non-owning (owned by the World).
     */
    explicit Fruit(const std::shared_ptr<logic::EntityModel>& model);

    /**
     * @brief Receives model events (unused for Fruit).
     * @param event Event emitted by the observed model.
     */
    void onNotify(const logic::EventType& event) override {}

    /**
     * @brief Draws the fruit to the window.
     * @param window Render target to draw to.
     */
    void render(sf::RenderWindow& window) const override;
};
} // namespace representation

#endif // PACMANPROJECT_FRUIT_VIEW_H
