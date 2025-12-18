//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"

namespace logic {
    enum class GhostState { Chase, Scatter, Fear, Dead };

    enum class Direction { Up, Down, Right, Left };


    class EntityModel : public Subject {
    protected:
        float _x, _y;

    public:
        EntityModel(const float x, const float y) : _x(x), _y(y) {
        }

        EntityModel(const EntityModel &that);

        void update(float dt);

        void setPosition(float x, float y);

        [[nodiscard]] std::pair<double, double> getPosition() const;

        [[nodiscard]] float getHeight() const;

        [[nodiscard]] float getWidth() const;
    };
}


#endif //PACMANPROJECT_ENTITYMODEL_H
