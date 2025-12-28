//
// Created by Abdellah on 12/28/2025.
//

#pragma once

namespace logic {
enum class EventType {
    Moved,

    Died,
    Respawned,

    CoinEaten,
    FruitEaten,
    GhostEaten,

    LevelCleared,

    DirectionChangedLeft,
    DirectionChangedUp,
    DirectionChangedDown,
    DirectionChangedRight,
    FrightenedFlashingStarted,
    FrightenedEnded,
    FrightenedStarted,
    PositionChanged
};

} // namespace logic
