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

        Collected,

        GhostStateChanged,

        ScoreChanged,
        LivesChanged,
        LevelChanged,

        FearModeStarted,
        FearModeEnding,
        FearModeEnded
    };


    class Event {
    };
}


#endif //PACMANPROJECT_EVENT_H
