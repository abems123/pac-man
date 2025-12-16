//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_STOPWATCH_H
#define PACMANPROJECT_STOPWATCH_H
#include <chrono>


namespace logic {
    class Stopwatch {
        std::chrono::steady_clock::time_point _last;
        float _delta_time = 0;

        Stopwatch() = default;

    public:
        static Stopwatch &getInstance();

        void tick();

        [[nodiscard]] float dt() const { return _delta_time; }
    };
}


#endif //PACMANPROJECT_STOPWATCH_H
