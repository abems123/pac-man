//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_PACMAN_H
#define PACMANPROJECT_PACMAN_H
#include "EntityView.h"


namespace representation {
    class PacMan : public EntityView {

        // bool standing = true;

        std::vector<sf::IntRect> _up_frames;
        std::vector<sf::IntRect> _down_frames;
        std::vector<sf::IntRect> _right_frames;
        std::vector<sf::IntRect> _left_frames;

        float _animation_timer = 0.f;
        float _frame_duration = 0.1f;
        int _current_frame = 0;
    public:
        explicit PacMan(const std::shared_ptr<logic::EntityModel> &model);

        void updateDirectionFrames();

        void onNotify(const logic::EventType &event) override;

        void render(sf::RenderWindow &window) const override;

        void update(float dt) override;




        // void updateAnimation(float dt) override;

        // void updateDirection() override;
    };
}


#endif //PACMANPROJECT_PACMAN_H
