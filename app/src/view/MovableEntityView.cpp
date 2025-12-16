//
// Created by abdellah on 12/13/25.
//

#include "../../include/view/MovableEntityView.h"

#include "entities/MovableEntityModel.h"


namespace representation {
    void MovableEntityView::update(const float dt) {
        EntityView::update(dt);
    }

    void MovableEntityView::updateDirection() {
        /*
        const auto dir = static_cast<std::shared_ptr<logic::MovableEntityModel>>(_model)->getDirection();
        if (dir != current_direction) {
            current_direction = dir;

            switch (dir) {
                case logic::Direction::Up: _frames = frames_up;
                    break;
                case logic::Direction::Down: _frames = frames_down;
                    break;
                case logic::Direction::Left: _frames = frames_left;
                    break;
                case logic::Direction::Right: _frames = frames_right;
                    break;
                default: break;
            }
            _current_frame = 0;
            _sprite.setTextureRect(_frames[_current_frame]);
        }
        */
    }

    void MovableEntityView::updateAnimation(const float dt) {
        _animation_timer += dt;

        if (_animation_timer >= _frame_duration) {
            _animation_timer = 0.f;
            _current_frame = (_current_frame + 1) % _frames.size();
            _sprite.setTextureRect(_frames[_current_frame]);
        }
    }
}
