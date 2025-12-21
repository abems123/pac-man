//
// Created by abdellah on 12/20/25.
//

#ifndef PACMANPROJECT_MOVABLEENTITYVIEW_H
#define PACMANPROJECT_MOVABLEENTITYVIEW_H
#include "EntityView.h"

namespace representation {
    class MovableEntityView : public EntityView {
    protected:
        bool is_moving = false;

        std::vector<sf::IntRect> _up_frames;
        std::vector<sf::IntRect> _down_frames;
        std::vector<sf::IntRect> _right_frames;
        std::vector<sf::IntRect> _left_frames;

        float _animation_timer = 0.f;
        float _frame_duration = 0.05f;
        int _current_frame = 0;

    public:
        MovableEntityView(const std::shared_ptr<logic::MovableEntityModel> &model, const std::vector<int> &columns,
                          const std::vector<int> &rows, const std::vector<int> &frames_number);

        void updatePosition();

        void onNotify(const logic::EventType &event) override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) const override;

        ~MovableEntityView() override;
    };
} // representation

#endif //PACMANPROJECT_MOVABLEENTITYVIEW_H
