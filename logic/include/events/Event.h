//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_EVENT_H
#define PACMANPROJECT_EVENT_H

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

        DirectionChangedLeft, DirectionChangedUp, DirectionChangedDown, DirectionChangedRight
    };


    class Event {
    };
}


#endif //PACMANPROJECT_EVENT_H
