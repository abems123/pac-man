//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_FRUIT_H
#define PACMANPROJECT_FRUIT_H
#include "../../logic/include/utils/Constants.h"
#include "Collectable.h"

namespace logic {
class Fruit : public Collectable {
public:
    Fruit(const float x, const float y) : Collectable(x, y, Constants::FRUIT_SCORE, EntityType::Fruit) {};
};
} // namespace logic

#endif // PACMANPROJECT_FRUIT_H
