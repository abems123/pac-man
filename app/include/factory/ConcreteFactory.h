//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_CONCRETEFACTORY_H
#define PACMANPROJECT_CONCRETEFACTORY_H

#include <vector>

#include "patterns/AbstractFactory.h"

namespace representation {
class EntityView;
class ResourceManager;

class ConcreteFactory : public logic::AbstractFactory {

    std::vector<std::shared_ptr<EntityView>>* _views;
    int _created_ghosts = 0;

public:
    explicit ConcreteFactory(std::vector<std::shared_ptr<EntityView>>* views);

    ~ConcreteFactory() override;

    std::shared_ptr<logic::PacMan> createPacMan(float x, float y) override;

    std::shared_ptr<logic::Ghost> createGhost(float x, float y) override;

    std::shared_ptr<logic::Coin> createCoin(float x, float y) override;

    std::shared_ptr<logic::Fruit> createFruit(float x, float y) override;

    std::shared_ptr<logic::Wall> createWall(float x, float y) override;
};
} // namespace representation

#endif // PACMANPROJECT_CONCRETEFACTORY_H
