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
    class Camera;


    class ConcreteFactory : public logic::AbstractFactory {
        Camera &camera;

        std::vector<std::shared_ptr<EntityView>> *_views;
    public:
        explicit ConcreteFactory(Camera &camera);

        ~ConcreteFactory() override;

        std::shared_ptr<logic::PacMan> createPacMan(float x, float y) override;

        std::shared_ptr<logic::Ghost> createLockedGhost(float x, float y) override;

        std::shared_ptr<logic::Ghost> createPredictiveGhost(float x, float y) override;

        std::shared_ptr<logic::Ghost> createDirectChaseGhost(float x, float y) override;

        std::shared_ptr<logic::Coin> createCoin(float x, float y) override;

        std::shared_ptr<logic::Fruit> createFruit(float x, float y) override;

        std::shared_ptr<logic::Wall> createWall(float x, float y) override;

        void setViewList(std::vector<std::shared_ptr<EntityView>> *views);
    };
}

#endif //PACMANPROJECT_CONCRETEFACTORY_H
