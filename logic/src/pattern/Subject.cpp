//
// Created by abdellah on 12/11/25.
//

#include "patterns/Subject.h"

#include "patterns/Observer.h"

namespace logic {
    void Subject::notify(const Event &e) const {
        for (auto &wobs: observers) {
            if (const auto obs = wobs.lock())
                obs->onNotify(e); // <-- View receives the event
        }
    }
}
