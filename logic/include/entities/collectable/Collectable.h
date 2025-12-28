//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_COLLECTABLE_H
#define PACMANPROJECT_COLLECTABLE_H
#include "entities/EntityModel.h"

class Collectable : public logic::EntityModel {
    int score;

public:
    Collectable(const float x, const float y, const int score, logic::EntityType type)
        : EntityModel(x, y, type), score(score) {}

    [[nodiscard]] int getScore() const { return score; }

private:
};

#endif // PACMANPROJECT_COLLECTABLE_H
