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


        // ====================== PacMan Animation [START] ======================
        if (!_frames.empty() && is_moving) {
            _animation_timer += dt;

            if (_animation_timer >= _frame_duration) {
                _animation_timer = 0.f;
                _current_frame = (_current_frame + 1) % _frames.size();
                _sprite.setTextureRect(_frames[_current_frame]);
            }
            is_moving = false;
        }
        // ====================== PacMan Animation [END] ======================
    }


    PacMan::PacMan(const std::shared_ptr<logic::EntityModel> &model)
        : EntityView(model, 17, 1, 2) {

        // TODO: Just hard code the dimensions of sprites since there is no rule to get them
        _sprites_sizes = {{33, 33}, {30, 33}, {23, 33}};

        AnimationManager::setFrames(17, 1, 2, _right_frames);
        AnimationManager::setFrames(17, 4, 2, _down_frames);
        AnimationManager::setFrames(17, 7, 2, _left_frames);
        AnimationManager::setFrames(17, 10, 2, _up_frames);
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
                is_moving = true;
                std::cout << "Pacman Moved" << std::endl;
                break;

            case logic::EventType::Stopped:
                is_moving = false;
                std::cout << "Pacman Stopped" << std::endl;
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
