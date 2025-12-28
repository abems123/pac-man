//
// Created by abdellah on 12/11/25.
//

#include "../../include/utils/Stopwatch.h"

namespace logic {
Stopwatch& Stopwatch::getInstance() {
    static Stopwatch inst;
    return inst;
}

void Stopwatch::tick() {
    const auto now = std::chrono::steady_clock::now();
    _delta_time = std::chrono::duration<float>(now - _last).count();
    _last = now;
}
} // namespace logic
