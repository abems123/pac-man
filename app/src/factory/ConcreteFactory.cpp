//
// Created by abdellah on 12/12/25.
//

#include "../../include/factory/ConcreteFactory.h"

ConcreteFactory::ConcreteFactory(representation::Camera& camera)
: camera(camera) {}

ConcreteFactory::~ConcreteFactory() = default;


logic::PacManModel* ConcreteFactory::createPacMan(float x, float y) {
    // temporary:
    return nullptr;
}

logic::Ghost* ConcreteFactory::createLockedGhost(float x, float y) {
    return nullptr;
}

logic::Ghost* ConcreteFactory::createPredictiveGhost(float x, float y) {
    return nullptr;
}

logic::Ghost* ConcreteFactory::createDirectChaseGhost(float x, float y) {
    return nullptr;
}

logic::Coin* ConcreteFactory::createCoin(float x, float y) {
    return nullptr;
}

logic::Fruit* ConcreteFactory::createFruit(float x, float y) {
    return nullptr;
}