//
// Created by abdellah on 12/20/25.
//

#pragma once

#include "view/EntityView.h"
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vector>

namespace representation {
/**
 * @brief Base view for entities that move and have directional animations.
 */
class MovableEntityView : public EntityView {
protected:
    /**
     * @brief Switch active animation frames.
     * @param frames New frame list to use.
     * @param reset If true, restart from frame 0.
     */
    void setAnimation(const std::vector<sf::IntRect>& frames, bool reset = true);

    /** @brief Directional frames (right). */
    std::vector<sf::IntRect> _right_frames;
    /** @brief Directional frames (down). */
    std::vector<sf::IntRect> _down_frames;
    /** @brief Directional frames (left). */
    std::vector<sf::IntRect> _left_frames;
    /** @brief Directional frames (up). */
    std::vector<sf::IntRect> _up_frames;

    /** @brief Current frame index in _frames. */
    std::size_t _current_frame = 0;

    /** @brief Accumulated time for switching frames. */
    float _animation_timer = 0.f;

    /** @brief Duration per frame in seconds. */
    float _frame_duration = 0.12f;

    /** @brief True when we should advance the animation this tick. */
    bool is_moving = false;

public:
    /**
     * @brief Construct a movable view with directional sprite frames.
     * @param model Model to observe.
     * @param columns Texture atlas columns for {right,down,left,up}.
     * @param rows Texture atlas rows for {right,down,left,up}.
     * @param frames_number Number of frames for each direction.
     */
    MovableEntityView(const std::shared_ptr<logic::MovableEntityModel>& model, const std::vector<int>& columns,
                      const std::vector<int>& rows, const std::vector<int>& frames_number);

    /**
     * @brief Handle model events (movement + direction changes).
     * @param event Event type from the model.
     */
    void onNotify(const logic::EventType& event) override;

    /**
     * @brief Update animation timers.
     * @param dt Delta time in seconds.
     */
    void update(float dt) override;

    /**
     * @brief Render the entity.
     * @param window Render target.
     */
    void render(sf::RenderWindow& window) const override;

    /**
     * @brief Destructor.
     */
    ~MovableEntityView() override;
};
} // namespace representation
