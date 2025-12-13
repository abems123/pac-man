//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_SUBJECT_H
#define PACMANPROJECT_SUBJECT_H
#include <memory>
#include <vector>


namespace logic {
    class Event;
    class Observer;

    /**
    * @brief Base class for the Subject in the Observer pattern.
    *
    * A Subject keeps track of a list of Observers and can notify them whenever
    * its state changes. Logic-side model classes (like PacMan or Ghost) inherit
     * from Subject so that their Views and the Score can react to updates.
    *
    * Note on raw pointers:
    * Observers are stored as raw pointers because the Subject does not own them.
     * The owning side (usually the State that created the View) controls their
     * lifetime. Using smart pointers here would incorrectly imply ownership, so
    * raw pointers are appropriate for this non-owning relationship.
    */
    class Subject {
        std::vector<Observer *> observers;

    public:
        void attach(Observer *obs) {
            if (obs) observers.push_back(obs);
        }

    protected:
        void notify(const Event &e) const;
    };
}


#endif //PACMANPROJECT_SUBJECT_H
