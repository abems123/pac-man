//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_WALL_VIEW
#define PACMANPROJECT_WALL_VIEW
#include <SFML/Graphics/RectangleShape.hpp>

#include "EntityView.h"


namespace representation {
    class Wall : public EntityView {
        sf::RectangleShape _shape;

    public:
        explicit Wall(const std::shared_ptr<logic::EntityModel> &model);

        void onNotify(const logic::EventType &event) override;

        void update(float dt) override;

        void render(sf::RenderWindow &window) const override;

        ~Wall() override = default;

    };
}

#endif //PACMANPROJECT_WALL_VIEW
