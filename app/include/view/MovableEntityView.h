//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_MOVABLEENTITYVIEW_H
#define PACMANPROJECT_MOVABLEENTITYVIEW_H
#include "EntityView.h"
#include "entities/EntityModel.h"


namespace logic {
    class PacMan;
    enum class Direction;
}

namespace representation {
    class MovableEntityView : public EntityView {
    protected:
        std::vector<sf::IntRect> frames_up;
        std::vector<sf::IntRect> frames_down;
        std::vector<sf::IntRect> frames_left;
        std::vector<sf::IntRect> frames_right;

        logic::Direction current_direction = logic::Direction::None;


    public:
        using EntityView::EntityView;

        void update(float dt) override;

        virtual void updateAnimation(float dt);

        virtual void updateDirection();
    };
}
#endif //PACMANPROJECT_MOVABLEENTITYVIEW_H
