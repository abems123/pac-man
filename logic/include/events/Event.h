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

        ScoreChanged,
        LivesChanged,
        LevelChanged,

        FearModeStarted,
        FearModeEnding,
        FearModeEnded,

        PacManMoved
    };


    class Event {
    };
}


#endif //PACMANPROJECT_EVENT_H
