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
class PacMan final : public MovableEntityView {

    /** Single-frame idle animation (mouth closed). */
    std::vector<sf::IntRect> _idle_frames;

public:
    /** Builds the Pac-Man view and sets the idle frame (mouth closed). */
    explicit PacMan(const std::shared_ptr<logic::PacMan>& model);

    /** Reacts to model events to update position and animations.
     *  @param event The event emitted by the model.
     */
    void onNotify(const logic::EventType& event) override;

    ~PacMan() override = default;
};
} // namespace representation

#endif // PACMANPROJECT_PACMAN_H
