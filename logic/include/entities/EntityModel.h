//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"
#include "utils/Direction.h"

namespace logic {
    /** Position must be in [-1:1]
     * So I'm not allowed to use pixels here
     ***/
    struct Position {
        float x, y;
    };

    class EntityModel : public Subject {
        Position position = {};
        Direction direction;
        float speed = 0;
        bool active = true;

    public:
        void update(float dt);

        void move(float dx, float dy);

        void setPosition(float x, float y);

        [[nodiscard]] Position getPosition() const;

        [[nodiscard]] Direction getDirection() const;

        void setDirection(Direction direction);
    };
}



#endif //PACMANPROJECT_ENTITYMODEL_H
