//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_LOCKEDGHOST_H
#define PACMANPROJECT_LOCKEDGHOST_H
#include "Ghost.h"

namespace logic {
class LockedGhost final : public Ghost {
public:
    /** @brief Inherits Ghost constructors. */
    using Ghost::Ghost;

    /**
     * @brief Constructs a locked ghost at the given world position.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     */
    LockedGhost(float x, float y);

    /**
     * @brief Computes the current target (unused for LockedGhost).
     * @return void
     */
    void computeTarget() override {}

    /**
     * @brief Selects the next movement direction for the locked ghost.
     * @return void
     */
    void decideDirection() override;

    /**
     * @brief Destructor.
     */
    ~LockedGhost() override;
};
} // namespace logic

#endif // PACMANPROJECT_LOCKEDGHOST_H
