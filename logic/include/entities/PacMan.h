//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_PACMANMODEL_H
#define PACMANPROJECT_PACMANMODEL_H
#include "EntityModel.h"
#include "MovableEntityModel.h"


namespace logic {
    class PacMan : public MovableEntityModel {
    public:
        using MovableEntityModel::MovableEntityModel;

        PacMan(float x, float y): MovableEntityModel(x, y, EntityType::Pacman) {};

    };
}


#endif //PACMANPROJECT_PACMANMODEL_H
