//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/Ghost.h"

#include "entities/ghost/LockedGhost.h"
#include "utils/AnimationManager.h"

namespace representation {
Ghost::Ghost(const std::shared_ptr<logic::Ghost>& model, int ghost_col)
    : MovableEntityView(model, {ghost_col, ghost_col, ghost_col, ghost_col}, {0, 2, 4, 6}, {2, 2, 2, 2}) {
    // =========== Tiny scale tweak so it fits nicer [START] ===========
    _sprite.setScale(_sprite.getScale().x - _sprite.getScale().x * 0.01,
                     _sprite.getScale().y - _sprite.getScale().y * 0.01);
    // =========== Tiny scale tweak so it fits nicer [END] ===========

    // =========== Load frightened visuals [START] ===========
    AnimationManager::setFrames(0, 11, 8, _frightened_frames); // blue
    AnimationManager::setFrames(1, 11, 8, _returning_frames);  // white flashing
    // =========== Load frightened visuals [END] ===========
}

void Ghost::onNotify(const logic::EventType& event) {
    // =========== Frightened visuals (blue/white/end) [START] ===========
    if (event == logic::EventType::FrightenedStarted) {
        _in_frightened = true;
        setAnimation(_frightened_frames, true);
        return;
    }

    if (event == logic::EventType::FrightenedFlashingStarted) {
        _in_frightened = true;
        setAnimation(_returning_frames, true);
        return;
    }

    if (event == logic::EventType::FrightenedEnded) {
        _in_frightened = false;

        // Put back the correct directional frames immediately.
        if (auto var = _model.lock()) {
            auto ghost_model = std::dynamic_pointer_cast<logic::Ghost>(var);
            if (ghost_model) {
                switch (ghost_model->getDirection()) {
                case logic::Direction::Left:
                    setAnimation(_left_frames, true);
                    break;
                case logic::Direction::Right:
                    setAnimation(_right_frames, true);
                    break;
                case logic::Direction::Up:
                    setAnimation(_up_frames, true);
                    break;
                case logic::Direction::Down:
                    setAnimation(_down_frames, true);
                    break;
                default:
                    break;
                }
            }
        }
        return;
    }
    // =========== Frightened visuals (blue/white/end) [END] ===========

    // =========== Keep movement animation even while frightened [START] ===========
    if (event == logic::EventType::Moved) {
        is_moving = true;
        updatePosition();
        return;
    }
    // =========== Keep movement animation even while frightened [END] ===========

    // =========== During frightened, ignore direction-change animations [START] ===========
    if (_in_frightened)
        return;
    // =========== During frightened, ignore direction-change animations [END] ===========

    MovableEntityView::onNotify(event);
}

void Ghost::update(float dt) { MovableEntityView::update(dt); }
void Ghost::render(sf::RenderWindow& window) const { MovableEntityView::render(window); }
} // namespace representation
