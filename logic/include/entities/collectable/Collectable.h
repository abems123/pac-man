//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_COLLECTABLE_H
#define PACMANPROJECT_COLLECTABLE_H
#include "entities/EntityModel.h"


class Collectable: public logic::EntityModel{
public:
    Collectable(const float x, const float y)
        : EntityModel(x, y) {
    }



private:
    bool is_collected = false;

};


#endif //PACMANPROJECT_COLLECTABLE_H