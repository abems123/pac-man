//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Fruit.h"

namespace representation {
representation::Fruit::Fruit(const std::shared_ptr<logic::EntityModel>& model) : EntityView(model, 12, 0, 1) {}
void Fruit::onNotify(const logic::EventType& event) {}

void Fruit::update(float dt) { EntityView::update(dt); }

void Fruit::render(sf::RenderWindow& window) const { EntityView::render(window); }
} // namespace representation
