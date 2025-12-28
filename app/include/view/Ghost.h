//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_GHOST_H
#define PACMANPROJECT_GHOST_H
#include "EntityView.h"
#include "MovableEntityView.h"

namespace logic {
class Ghost;
}

namespace representation {
/**
 * @brief Ghost view that supports frightened visuals (blue + white flashing).
 */
class Ghost final : public MovableEntityView {
public:
    /**
     * @brief Construct a ghost view using the ghost model.
     * @param model Ghost model to observe.
     * @param ghost_col Sprite sheet column of this ghost (normal mode).
     */
    Ghost(const std::shared_ptr<logic::Ghost>& model, int ghost_col);

    /**
     * @brief Handle events from the model (including frightened start/flash/end).
     * @param event Event type from the model.
     */
    void onNotify(const logic::EventType& event) override;

    /**
     * @brief Update the view (animation).
     * @param dt Delta time in seconds.
     */
    void update(float dt) override;

    /**
     * @brief Render the ghost.
     * @param window Render target.
     */
    void render(sf::RenderWindow& window) const override;

private:
    /** @brief Frames for frightened (blue) animation. */
    std::vector<sf::IntRect> _frightened_frames;

    /** @brief Frames for flashing/white animation (last seconds). */
    std::vector<sf::IntRect> _frightened_flash_frames;

    /** @brief True while we are showing frightened visuals (blue/white). */
    bool _in_frightened = false;
};
} // namespace representation

#endif // PACMANPROJECT_GHOST_H
