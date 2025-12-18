//
// Created by abdellah on 12/12/25.
//

#include "../../include/factory/ConcreteFactory.h"

#include <memory>

#include "camera/Camera.h"
#include "entities/PacMan.h"
#include "entities/Wall.h"
#include "entities/ghost/Ghost.h"
#include "entities/ghost/LockedGhost.h"
#include "patterns/AbstractFactory.h"
#include "utils/ResourceManager.h"
#include "view/Coin.h"
#include "view/Ghost.h"
#include "view/PacMan.h"
#include "view/Wall.h"
#include "entities/collectable/Coin.h"
#include "entities/collectable/Fruit.h"
#include "patterns/AbstractFactory.h"
#include "view/Fruit.h"

namespace representation {
    ConcreteFactory::ConcreteFactory(std::vector<std::shared_ptr<EntityView> > *views)
        :  _views(views) {
    }

    ConcreteFactory::~ConcreteFactory() = default;


    std::shared_ptr<logic::PacMan> ConcreteFactory::createPacMan(float x, float y) {
        auto model = std::make_shared<logic::PacMan>(x, y);
        auto view = std::make_shared<PacMan>(model);
        model->attach(view);
        _views->push_back(view);
        return model;
    }

    std::shared_ptr<logic::Wall> ConcreteFactory::createWall(float x, float y) {
        auto model = std::make_shared<logic::Wall>(x, y);
        auto view = std::make_shared<Wall>(model);
        model->attach(view);
        _views->push_back(view);
        return model;
    }


    std::shared_ptr<logic::Ghost> ConcreteFactory::createLockedGhost(float x, float y) {
        // const sf::Texture &texture = ResourceManager::instance().getTexture();
        // auto model = std::make_shared<logic::LockedGhost>(x, y);
        // auto view = std::make_shared<Ghost>(model, texture, camera);
        // model->attach(view);
        // _views->push_back(view);
        // return model;
        return nullptr;
    }

    std::shared_ptr<logic::Ghost> ConcreteFactory::createPredictiveGhost(float x, float y) {
        return nullptr;
    }

    std::shared_ptr<logic::Ghost> ConcreteFactory::createDirectChaseGhost(float x, float y) {
        return nullptr;
    }

    std::shared_ptr<logic::Coin> ConcreteFactory::createCoin(float x, float y) {
        auto model = std::make_shared<logic::Coin>(x, y);
        auto view = std::make_shared<Coin>(model);
        model->attach(view);
        _views->push_back(view);
        return model;
    }

    std::shared_ptr<logic::Fruit> ConcreteFactory::createFruit(float x, float y) {
        auto model = std::make_shared<logic::Fruit>(x, y);
        auto view = std::make_shared<Fruit>(model);
        model->attach(view);
        _views->push_back(view);
        return model;
    }

}
