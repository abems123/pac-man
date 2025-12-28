//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/PacMan.h"


#include "../../logic/include/entities/PacMan.h"

#include <events/EventType.h>
#include "utils/AnimationManager.h"

namespace representation {
PacMan::PacMan(const std::shared_ptr<logic::PacMan>& model)
    : MovableEntityView(model, {17, 17, 17, 17}, {1, 4, 7, 10}, {2, 2, 2, 2}) {

    // =========== Idle frame (mouth closed) [START] ===========
    AnimationManager::setFrames(17, 0, 1, _idle_frames);
    setAnimation(_idle_frames, false);
    _sprite.setTextureRect(_idle_frames.front());
    // =========== Idle frame (mouth closed) [END] ===========
}

void PacMan::onNotify(const logic::EventType& event) {
    if (event == logic::EventType::Respawned) {
        _sprite.setTextureRect(_idle_frames.front());
        return;
    }

    MovableEntityView::onNotify(event);
}

} // namespace representation
