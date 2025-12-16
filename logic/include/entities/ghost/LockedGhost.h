//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_LOCKEDGHOST_H
#define PACMANPROJECT_LOCKEDGHOST_H
#include "Ghost.h"

namespace logic {
    class LockedGhost : public Ghost {
    public:
        void computeTarget(World *world, PacMan *pac_man) override;

        void stepTowardTarget(World *world) override;

        ~LockedGhost() override;

        using Ghost::Ghost;


    };
}


#endif //PACMANPROJECT_LOCKEDGHOST_H
