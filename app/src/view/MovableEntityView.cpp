//
// Created by abdellah on 12/20/25.
//

#include "../../include/view/MovableEntityView.h"

#include "utils/AnimationManager.h"

namespace representation {
    MovableEntityView::MovableEntityView(const std::shared_ptr<logic::MovableEntityModel> &model,
                                         const std::vector<int> &columns, const std::vector<int> &rows,
                                         const std::vector<int> &frames_number) : EntityView(model) {

        AnimationManager::setFrames(columns[0], rows[0], frames_number[0], _right_frames);
        AnimationManager::setFrames(columns[1], rows[1], frames_number[1], _down_frames);
        AnimationManager::setFrames(columns[2], rows[2], frames_number[2], _left_frames);
        AnimationManager::setFrames(columns[3], rows[3], frames_number[3], _up_frames);

        _frames = _right_frames;
        _sprite.setTextureRect(_frames.front());
    }

    void MovableEntityView::updatePosition() {
        if (const auto var = _model.lock()) {
            auto [x,y] = Camera::instance().worldToScreen(var->getPosition().first, var->getPosition().second);
            _sprite.setPosition(x, y);
        }
    }

    void MovableEntityView::onNotify(const logic::EventType &event) {
        switch (event) {
            case logic::EventType::Moved:
                is_moving = true;
                updatePosition();
                break;

            case logic::EventType::DirectionChangedLeft:
                _frames = _left_frames;
                break;
            case logic::EventType::DirectionChangedUp:
                _frames = _up_frames;
                break;
            case logic::EventType::DirectionChangedDown:
                _frames = _down_frames;
                break;
            case logic::EventType::DirectionChangedRight:
                _frames = _right_frames;
                break;
        }
    }

    void MovableEntityView::update(float dt) {
        // ====================== Moveable Animation [START] ======================
        if (!_frames.empty() && is_moving) {
            _animation_timer += dt;

            if (_animation_timer >= _frame_duration) {
                _animation_timer = 0.f;
                _current_frame = (_current_frame + 1) % _frames.size();
                _sprite.setTextureRect(_frames[_current_frame]);
            }
            is_moving = false;
        }
        // ====================== Moveable Animation [END] ======================
    }

    void MovableEntityView::render(sf::RenderWindow &window) const {
        EntityView::render(window);
    }

    MovableEntityView::~MovableEntityView() {
    }
} // representation
