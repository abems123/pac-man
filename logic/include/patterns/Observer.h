//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_OBSERVER_H
#define PACMANPROJECT_OBSERVER_H

namespace logic {
enum class EventType;
class Event;

/**
 * @brief Observer interface for receiving model events.
 */
class Observer {
public:
    /** @brief Virtual destructor for safe polymorphic cleanup. */
    virtual ~Observer() = default;

    /**
     * @brief Called by a Subject when an event occurs.
     * @param event Event type being emitted by the Subject.
     * @return Nothing.
     */
    virtual void onNotify(const EventType& event) = 0;
};
} // namespace logic

#endif // PACMANPROJECT_OBSERVER_H
