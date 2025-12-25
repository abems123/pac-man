//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_MOVABLEENTITYMODAL_H
#define PACMANPROJECT_MOVABLEENTITYMODAL_H
#include "EntityModel.h"
#include "events/Event.h"

namespace logic {
class World;
class MovableEntityModel : public EntityModel {
protected:
    float _speed = 0.2f;
    Direction _direction = Direction::Right;

    // We need an instance of World to handle
    World* _world = nullptr;

    bool _is_moving = false;

public:
    using EntityModel::EntityModel;

    MovableEntityModel(const MovableEntityModel& that);
    MovableEntityModel& operator=(const MovableEntityModel& that);

    [[nodiscard]] float getSpeed() const { return _speed; }
    void setSpeed(const float speed) { this->_speed = speed; }

    [[nodiscard]] Direction getDirection() const { return _direction; };

    void setDirection(Direction direction);
    void setMoving(const bool moving) { _is_moving = moving; }

    virtual void update(float dt);
    void move(float dt);
};
} // namespace logic

#endif // PACMANPROJECT_MOVABLEENTITYMODAL_H
