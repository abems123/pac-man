//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/PacMan.h"

#include <iostream>

#include "../../logic/include/entities/EntityModel.h"
#include "../../logic/include/entities/PacMan.h"

#include "camera/Camera.h"
#include "events/Event.h"
#include "utils/AnimationManager.h"
#include "utils/Constants.h"


namespace representation {
    void PacMan::update(const float dt) {
        EntityView::update(dt);

        // This is used only to update the animation, the movements and direction changing is handeled by onNotify

        // animation…
        if (!_frames.empty()) {
            _animation_timer += dt;

            if (_animation_timer >= _frame_duration) {
                _animation_timer = 0.f;
                _current_frame = (_current_frame + 1) % _frames.size();
                _sprite.setTextureRect(_frames[_current_frame]);
            }
        }
    }


    PacMan::PacMan(const std::shared_ptr<logic::EntityModel> &model)
        : EntityView(model, 17, 0, 3) {
        AnimationManager::setFrames(17, 0, 3, _right_frames);
        AnimationManager::setFrames(17, 3, 3, _down_frames);
        AnimationManager::setFrames(17, 6, 3, _left_frames);
        AnimationManager::setFrames(17, 9, 3, _up_frames);
    }

    void PacMan::updateDirectionFrames() {
        if (auto var = _model.lock()) {
            auto pacmanModel = std::dynamic_pointer_cast<logic::MovableEntityModel>(var);

            if (!pacmanModel) {
                throw std::logic_error("PacManView has non-movable model");
            }

            switch (pacmanModel->getDirection()) {
                case logic::Direction::Left:
                    _frames = _left_frames;
                    break;
                case logic::Direction::Right:
                    _frames = _right_frames;
                    break;
                case logic::Direction::Up:
                    _frames = _up_frames;
                    break;
                case logic::Direction::Down:
                    _frames = _down_frames;
                    break;
            }
        }
    }

    void PacMan::onNotify(const logic::EventType &event) {
        switch (event) {
            case logic::EventType::DirectionChanged:
                updateDirectionFrames();
                std::cout << "Direction Changed" << std::endl;
                break;

            case logic::EventType::Moved:
                // updateScreenPosition();
                std::cout << "Pacman Moved" << std::endl;
                break;

                /*
            case logic::EventType::PowerChanged:
                updatePowerColor();
                break;

            case logic::EventType::Died:
                startDeathAnimation();
                break;
                */
        }
    }

    void PacMan::render(sf::RenderWindow &window) const {
        EntityView::render(window);
    }
}
