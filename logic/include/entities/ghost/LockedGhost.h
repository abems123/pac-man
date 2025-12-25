//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_LOCKEDGHOST_H
#define PACMANPROJECT_LOCKEDGHOST_H
#include "Ghost.h"

namespace logic {
    class LockedGhost : public Ghost {
    public:
        using Ghost::Ghost;

        LockedGhost(float x, float y);

        void computeTarget() override {}

        void stepTowardTarget() override {}

        void decideDirection() override;

        bool isAtTileCenter(int row, int col, float epsX, float epsY) const;

        ~LockedGhost() override;


    };
}


#endif //PACMANPROJECT_LOCKEDGHOST_H
