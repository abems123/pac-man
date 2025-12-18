//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Coin.h"

#include "utils/AnimationManager.h"

namespace representation {
    representation::Coin::Coin(const std::shared_ptr<logic::EntityModel> &model) : EntityView(model, 8, 5, 1) {
    }

    void Coin::onNotify(const logic::EventType &event) {
    }

    void Coin::render(sf::RenderWindow &window) const {
        EntityView::render(window);
    }

    void Coin::update(float dt) {
        EntityView::update(dt);
    }
}
