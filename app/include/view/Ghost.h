//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_GHOST_H
#define PACMANPROJECT_GHOST_H
#include "EntityView.h"
#include "MovableEntityView.h"


namespace logic {
    class Ghost;
}

namespace representation {
    class Ghost : public MovableEntityView {
    public:
        Ghost(const std::shared_ptr<logic::Ghost> &model, int ghost_col);

        void onNotify(const logic::EventType &event) override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) const override;
    };
}


#endif //PACMANPROJECT_GHOST_H
