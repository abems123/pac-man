//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_SUBJECT_H
#define PACMANPROJECT_SUBJECT_H
#include <memory>
#include <vector>
#include <algorithm>


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
        std::vector<std::weak_ptr<Observer> > observers;

    public:
        virtual ~Subject() = default;

        Subject() = default;
        Subject(const Subject &that);

        void attach(const std::weak_ptr<Observer> &obs) {
            if (const auto var = obs.lock()) observers.push_back(var);
        }

        void detach(const std::weak_ptr<Observer> &obs) {
            observers.erase(
                std::ranges::remove_if(observers,
                                       [](const std::weak_ptr<Observer>& w) { return w.expired(); }).begin(),
                observers.end()
            );
        }

    protected:
        void notify(const EventType &e) const;
    };
}


#endif //PACMANPROJECT_SUBJECT_H
