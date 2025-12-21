//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_FRUIT_H
#define PACMANPROJECT_FRUIT_H
#include "Collectable.h"
#include "../../logic/include/utils/Constants.h"

namespace logic {
    class Fruit : public Collectable {
    public:
        Fruit(const float x, const float y) : Collectable(x, y, Constants::FRUIT_SCORE) {
        };
    };
}


#endif //PACMANPROJECT_FRUIT_H
