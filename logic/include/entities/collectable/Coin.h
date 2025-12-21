//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_COIN_H
#define PACMANPROJECT_COIN_H
#include "Collectable.h"
#include "../../logic/include/utils/Constants.h"


namespace logic {
    class Coin : public Collectable {
    public:
        Coin(const float x, const float y) : Collectable(x, y, Constants::COIN_SCORE) {
        }
    };
}


#endif //PACMANPROJECT_COIN_H
