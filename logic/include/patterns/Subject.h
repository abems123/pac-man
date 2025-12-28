//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_SUBJECT_H
#define PACMANPROJECT_SUBJECT_H
#include <memory>
#include <vector>

namespace logic {
enum class EventType;
class Event;
class Observer;

/**
 * @brief Base class for the Subject in the Observer pattern.
 *
 * A Subject keeps track of a list of Observers and can notify them whenever
 * its state changes. Logic-side model classes (like PacMan or Ghost) inherit
 * from Subject so that their Views and the Score can react to updates.
 *
 */
class Subject {
    std::vector<std::weak_ptr<Observer>> observers;

public:
    Subject() = default;
    virtual ~Subject() = default;

    /** Copying a Subject is disabled to avoid duplicating observer subscriptions. */
    Subject(const Subject&) = delete;

    /** Copy assignment is disabled for the same reason. */
    Subject& operator=(const Subject&) = delete;

    /** Moving is allowed; the moved-to subject keeps the observer list. */
    Subject(Subject&&) noexcept = default;

    /** Move assignment is allowed; the moved-to subject keeps the observer list. */
    Subject& operator=(Subject&&) noexcept = default;

    void attach(const std::weak_ptr<Observer>& obs) {
        if (const auto var = obs.lock())
            observers.push_back(var);
    }

    void notify(const EventType& e) const;
};
} // namespace logic

#endif // PACMANPROJECT_SUBJECT_H
