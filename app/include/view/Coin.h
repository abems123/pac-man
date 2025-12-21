//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_COIN_VIEW_H
#define PACMANPROJECT_COIN_VIEW_H
#include "EntityView.h"

namespace logic {
    class Coin;
}

namespace representation {
    class Coin : public EntityView {
        sf::CircleShape _shape;
    public:
        Coin(const std::shared_ptr<logic::Coin> &model);

        void onNotify(const logic::EventType &event) override;

        void render(sf::RenderWindow &window) const override;

        void update(float dt) override;
    };
}


#endif //PACMANPROJECT_COIN_VIEW_H
