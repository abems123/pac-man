//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_COIN_H
#define PACMANPROJECT_COIN_H
#include "../../logic/include/utils/Constants.h"
#include "Collectable.h"

namespace logic {
class Coin final : public Collectable {
public:
    Coin(const float x, const float y) : Collectable(x, y, Constants::COIN_SCORE_FASTEST, EntityType::Coin) {}
};
} // namespace logic

#endif // PACMANPROJECT_COIN_H
