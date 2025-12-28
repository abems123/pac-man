//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_OBSERVER_H
#define PACMANPROJECT_OBSERVER_H

namespace logic {
enum class EventType;
class Event;

class Observer {
public:
    virtual ~Observer() = default;

    void virtual onNotify(const EventType& event) = 0;
};
} // namespace logic

#endif // PACMANPROJECT_OBSERVER_H
