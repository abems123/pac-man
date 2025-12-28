//
// Created by abdellah on 12/11/25.
//

#include "patterns/Subject.h"

#include "patterns/Observer.h"

namespace logic {
    void Subject::notify(const EventType &e) const {
        for (auto &obs: observers) {
            if (const auto ob = obs.lock()) {
                ob->onNotify(e);
            }
        }
    }
}
