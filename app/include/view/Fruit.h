//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_FRUIT_VIEW_H
#define PACMANPROJECT_FRUIT_VIEW_H
#include "EntityView.h"

namespace representation {
class Fruit : public EntityView {
public:
    explicit Fruit(const std::shared_ptr<logic::EntityModel>& model);

    void onNotify(const logic::EventType& event) override;

    void update(float dt) override;

    void render(sf::RenderWindow& window) const override;
};
} // namespace representation

#endif // PACMANPROJECT_FRUIT_VIEW_H