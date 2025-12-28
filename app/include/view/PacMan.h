//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_PACMAN_H
#define PACMANPROJECT_PACMAN_H
#include "MovableEntityView.h"

namespace logic {
class PacMan;
}

namespace representation {
class PacMan : public MovableEntityView {

public:
    explicit PacMan(const std::shared_ptr<logic::PacMan>& model);

    void updateDirectionFrames();

    void onNotify(const logic::EventType& event) override;

    void render(sf::RenderWindow& window) const override;

    void update(float dt) override;

    ~PacMan() override = default;

    // void updateAnimation(float dt) override;

    // void updateDirection() override;
};
} // namespace representation

#endif // PACMANPROJECT_PACMAN_H
