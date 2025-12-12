//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"
#include "utils/Direction.h"

namespace logic {
    struct Position {
        int x, y;
    };

    class EntityModel : public Subject {
        Position position = {};
        Direction direction;
        float speed = 0;
        bool active = true;

    public:
        void update(float dt);

        void move(int dx, int dy);

        void setPosition(int x, int y);

        [[nodiscard]] Position getPosition() const;

        [[nodiscard]] Direction getDirection() const;

        void setDirection(Direction direction);
    };
}



#endif //PACMANPROJECT_ENTITYMODEL_H
