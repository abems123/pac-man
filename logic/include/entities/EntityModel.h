//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"

namespace logic {
    enum class GhostState { Chase, Scatter, Fear, Dead };

    enum class Direction { Up, Down, Right, Left, None };

    struct Rectangle {
        float x; // left
        float y; // top (or bottom, depending on your convention)
        float w; // width
        float h; // height
    };

    class EntityModel : public Subject {
    protected:
        Rectangle bounds;

    public:
        EntityModel(const float x, const float y) {
            bounds.x = x;
            bounds.y = y;
        }

        EntityModel(const EntityModel &that);

        void update(float dt);

        void setPosition(float x, float y);

        void setHeight(float tile_height) { bounds.h = tile_height; };
        void setWidth(float tile_width) { bounds.w = tile_width; };

        [[nodiscard]] std::pair<double, double> getPosition() const;

        [[nodiscard]] float getHeight() const;

        [[nodiscard]] float getWidth() const;
    };
}


#endif //PACMANPROJECT_ENTITYMODEL_H
