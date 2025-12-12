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

    class Subject {
        std::vector<std::weak_ptr<Observer> > observers;

    public:
        void attach(const std::weak_ptr<Observer> &obs) {
            observers.push_back(obs);
        }

    protected:
        void notify(const Event &e) const;
    };
}



#endif //PACMANPROJECT_SUBJECT_H
