//
// Created by abdellah on 12/11/25.
//

#include "patterns/Subject.h"

#include "patterns/Observer.h"

namespace logic {
    void Subject::notify(const Event &e) const {
        for (auto &obs: observers) {
            if (obs) {
                // The Observer (View) receives the event
                obs->onNotify(e);
            }
        }
    }
}
