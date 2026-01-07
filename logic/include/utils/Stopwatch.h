//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_STOPWATCH_H
#define PACMANPROJECT_STOPWATCH_H
#include <chrono>

namespace logic {

/**
 * @brief Singleton stopwatch used to measure frame-to-frame delta time.
 */
class Stopwatch {
    /** @brief Timestamp of the previous tick(). */
    std::chrono::steady_clock::time_point _last;

    /** @brief Last computed delta time in seconds. */
    float _delta_time = 0.0f;

    /** @brief Private constructor for singleton usage. */
    Stopwatch() : _last(std::chrono::steady_clock::now()) {}

public:
    /**
     * @brief Returns the global Stopwatch instance.
     * @return Reference to the singleton instance.
     */
    static Stopwatch& getInstance();

    /**
     * @brief Updates the internal delta time using the current steady clock time.
     * @return Nothing.
     */
    void tick();

    /**
     * @brief Returns the last computed delta time.
     * @return Delta time in seconds since the previous tick().
     */
    [[nodiscard]] float dt() const { return _delta_time; }
};

} // namespace logic

#endif // PACMANPROJECT_STOPWATCH_H
