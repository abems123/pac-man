//
// Created by abdellah on 12/12/25.
//

#include "../../include/factory/ConcreteFactory.h"

#include <memory>
#include <stdexcept>

#include "../../include/view/Ghost.h"
#include "entities/PacMan.h"
#include "entities/Wall.h"
#include "entities/collectable/Coin.h"
#include "entities/collectable/Fruit.h"
#include "entities/ghost/DirectChaseGhost.h"
#include "entities/ghost/Ghost.h"
#include "entities/ghost/LockedGhost.h"
#include "view/Coin.h"
#include "view/Fruit.h"
#include "view/PacMan.h"
#include "view/Wall.h"

namespace representation {
ConcreteFactory::ConcreteFactory(std::vector<std::shared_ptr<EntityView>>& views) : _views(views) {}

ConcreteFactory::~ConcreteFactory() = default;

std::shared_ptr<logic::PacMan> ConcreteFactory::createPacMan(float x, float y) {
    auto model = std::make_shared<logic::PacMan>(x, y);
    const auto view = std::make_shared<PacMan>(model);
    model->attach(view);
    _views.push_back(view);
    return model;
}

std::shared_ptr<logic::Ghost> ConcreteFactory::createGhost(float x, float y) {
    using logic::GhostMode;

    std::shared_ptr<logic::Ghost> model;

    switch (_created_ghosts) {
    case 0:
        model = std::make_shared<logic::LockedGhost>(x, y);
        model->setMode(GhostMode::Chase);
        model->setReleaseDelay(0.f);
        break;

    case 1:
        model = std::make_shared<logic::LookaheadChaseGhost>(x, y);
        model->setMode(GhostMode::Chase);
        model->setReleaseDelay(0.f);
        break;

    case 2:
        model = std::make_shared<logic::LookaheadChaseGhost>(x, y);
        model->setMode(GhostMode::Center);
        model->setReleaseDelay(5.f);
        break;

    case 3:
        model = std::make_shared<logic::DirectChaseGhost>(x, y);
        model->setMode(GhostMode::Center);
        model->setReleaseDelay(10.f);
        break;

    default:
        throw std::runtime_error("Too many ghosts spawned (unexpected '@' in map).");
    }

    const auto view = std::make_shared<Ghost>(model, _created_ghosts);
    model->attach(view);
    _views.push_back(view);

    ++_created_ghosts;
    return model;
}

std::shared_ptr<logic::Wall> ConcreteFactory::createWall(float x, float y, const bool isGate) {
    auto model = std::make_shared<logic::Wall>(x, y);
    const auto view = std::make_shared<Wall>(model, isGate);
    model->attach(view);
    _views.push_back(view);
    return model;
}

std::shared_ptr<logic::Coin> ConcreteFactory::createCoin(float x, float y) {
    auto model = std::make_shared<logic::Coin>(x, y);
    const auto view = std::make_shared<Coin>(model);
    model->attach(view);
    _views.push_back(view);
    return model;
}

std::shared_ptr<logic::Fruit> ConcreteFactory::createFruit(float x, float y) {
    auto model = std::make_shared<logic::Fruit>(x, y);
    const auto view = std::make_shared<Fruit>(model);
    model->attach(view);
    _views.push_back(view);
    return model;
}
} // namespace representation
