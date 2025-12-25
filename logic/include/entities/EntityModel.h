//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"

namespace logic {
enum class GhostMode { Center, Chase, Fear, Dead };

enum class Direction { Up, Down, Right, Left, None };

enum class EntityType { Pacman, Wall, Coin, Fruit, LockedGhost, FollowerGhost, LookaheadChaseGhost };

class EntityModel : public Subject {
protected:
    EntityType _type;
    float _x, _y;

public:
    EntityModel(const float x, const float y, const EntityType type) : _type(type), _x(x), _y(y) {}

    EntityModel(const EntityModel& that);

    EntityModel& operator=(const EntityModel& that);

    void setPosition(float x, float y);

    std::pair<float, float> getPosition() const { return {_x, _y}; };

    [[nodiscard]] virtual EntityType getType() const { return _type; };
};
} // namespace logic

#endif // PACMANPROJECT_ENTITYMODEL_H
