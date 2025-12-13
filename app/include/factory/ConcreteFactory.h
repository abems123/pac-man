//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_CONCRETEFACTORY_H
#define PACMANPROJECT_CONCRETEFACTORY_H

#include "patterns/AbstractFactory.h"


namespace representation {
    class Camera;
}

class ConcreteFactory : public logic::AbstractFactory {
    representation::Camera &camera;

public:
    ConcreteFactory(representation::Camera &camera);

    logic::PacManModel *createPacMan(float x, float y) override;

    logic::Ghost *createLockedGhost(float x, float y) override;

    logic::Ghost *createPredictiveGhost(float x, float y) override;

    logic::Ghost *createDirectChaseGhost(float x, float y) override;

    logic::Coin *createCoin(float x, float y) override;

    logic::Fruit *createFruit(float x, float y) override;

    ~ConcreteFactory() override;
};


#endif //PACMANPROJECT_CONCRETEFACTORY_H
