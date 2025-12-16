//
// Created by abdellah on 12/11/25.
//

#include "patterns/Subject.h"

#include "patterns/Observer.h"

namespace logic {
    Subject::Subject(const Subject &that) {
        observers = that.observers;
    }

    void Subject::notify(const EventType &e) const {
        for (auto &obs: observers) {
            if (const auto ob = obs.lock()) {
                // The Observer (View) receives the event
                ob->onNotify(e);
            }
        }
    }
}
