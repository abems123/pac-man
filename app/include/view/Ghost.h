//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_GHOST_H
#define PACMANPROJECT_GHOST_H
#include "EntityView.h"


namespace representation {
    class Ghost : public EntityView {
    public:
        using EntityView::EntityView;

        void onNotify(const logic::EventType &event) override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) const override;
    };
}


#endif //PACMANPROJECT_GHOST_H
