//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_MOVABLEENTITYMODAL_H
#define PACMANPROJECT_MOVABLEENTITYMODAL_H
#include "EntityModel.h"
#include "events/Event.h"

namespace logic {
    class MovableEntityModel : public EntityModel {
    protected:
        float _speed = 0.5f;
        Direction _direction = Direction::Right;

    public:
        using EntityModel::EntityModel;

        MovableEntityModel(const MovableEntityModel &that);

        [[nodiscard]] float getSpeed() const { return _speed; }
        void setSpeed(const float speed) { this->_speed = speed; }

        [[nodiscard]] Direction getDirection() const { return _direction; };

        void setDirection(const Direction direction);

        void move(const float dx, const float dy);;
    };
}

#endif //PACMANPROJECT_MOVABLEENTITYMODAL_H
