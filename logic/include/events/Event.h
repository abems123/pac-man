//
// Created by abdellah on 12/11/25.
//

#pragma once

namespace logic {
    enum class EventType {
        Moved,
        DirectionChanged,

        Spawned,
        Died,
        Respawned,

        CoinEaten,
        FruitEaten,
        GhostEaten,

        LevelCleared,

        GhostStateChanged,

        LivesChanged,
        LevelChanged,

        FearModeStarted,
        FearModeEnding,
        FearModeEnded,

        DirectionChangedLeft, DirectionChangedUp, DirectionChangedDown, DirectionChangedRight,
    FrightenedFlashingStarted,
    FrightenedEnded,
    FrightenedStarted
};


    class Event {
    };
}


