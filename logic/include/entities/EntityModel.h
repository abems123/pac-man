//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"

namespace logic {
    enum class GhostMode { Center, Chase, Fear, Dead };

    enum class Direction { Up, Down, Right, Left };

    enum class EntityType { Pacman, Wall, Coin, Fruit, LockedGhost, PredictiveGhost, FollowerGhost };

    class EntityModel : public Subject {
    protected:
        float _x, _y;
        EntityType _type;

    public:
        EntityModel(const float x, const float y, EntityType type) : _x(x), _y(y), _type(type) {
        }

        EntityModel(const EntityModel &that);

        EntityModel &operator=(const EntityModel &that);

        void setPosition(float x, float y);

        [[nodiscard]] std::pair<float, float> getPosition() const;

        [[nodiscard]] virtual EntityType getType() const { return _type; };
    };
}


#endif //PACMANPROJECT_ENTITYMODEL_H
