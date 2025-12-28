//
// Created by abdellah on 12/20/25.
//

#include "../../include/view/MovableEntityView.h"

#include "utils/AnimationManager.h"

#include <events/EventType.h>

namespace representation {
MovableEntityView::MovableEntityView(const std::shared_ptr<logic::MovableEntityModel>& model,
                                     const std::vector<int>& columns, const std::vector<int>& rows,
                                     const std::vector<int>& frames_number)
    : EntityView(model) {
    // =========== Load frames from the sprite sheet [START] ===========
    AnimationManager::setFrames(columns[0], rows[0], frames_number[0], _right_frames);
    AnimationManager::setFrames(columns[1], rows[1], frames_number[1], _down_frames);
    AnimationManager::setFrames(columns[2], rows[2], frames_number[2], _left_frames);
    AnimationManager::setFrames(columns[3], rows[3], frames_number[3], _up_frames);
    // =========== Load frames from the sprite sheet [END] ===========

    // =========== Default to right frames [START] ===========
    setAnimation(_right_frames, true);
    // =========== Default to right frames [END] ===========
}
void MovableEntityView::setAnimation(const std::vector<sf::IntRect>& frames, bool reset) {
    // =========== Swap animation without weird leftovers [START] ===========
    _frames = frames;

    if (reset) {
        _current_frame = 0;
        _animation_timer = 0.f;
        if (!_frames.empty())
            _sprite.setTextureRect(_frames.front());
    }
    // =========== Swap animation without weird leftovers [END] ===========
}

void MovableEntityView::onNotify(const logic::EventType& event) {
    switch (event) {
    case logic::EventType::Moved:
        is_moving = true;
        updatePosition();
        break;
    case logic::EventType::PositionChanged:
        // =========== Teleport/snap without movement animation [START] ===========
        is_moving = false;
        updatePosition();
        // =========== Teleport/snap without movement animation [END] ===========
        break;
    case logic::EventType::DirectionChangedLeft:
        setAnimation(_left_frames, true);
        break;
    case logic::EventType::DirectionChangedUp:
        setAnimation(_up_frames, true);
        break;
    case logic::EventType::DirectionChangedDown:
        setAnimation(_down_frames, true);
        break;
    case logic::EventType::DirectionChangedRight:
        setAnimation(_right_frames, true);
        break;

    default:
        break;
    }
}

void MovableEntityView::update(float dt) {
    // =========== Animate only when something moved [START] ===========
    if (!_frames.empty() && is_moving) {
        _animation_timer += dt;

        if (_animation_timer >= _frame_duration) {
            _animation_timer = 0.f;
            _current_frame = (_current_frame + 1) % _frames.size();
            _sprite.setTextureRect(_frames[_current_frame]);
        }

        is_moving = false;
    }
    // =========== Animate only when something moved [END] ===========
}

void MovableEntityView::render(sf::RenderWindow& window) const { EntityView::render(window); }

MovableEntityView::~MovableEntityView() {}
} // namespace representation
