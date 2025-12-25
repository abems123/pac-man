//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ABSTRACTFACTORY_H
#define PACMANPROJECT_ABSTRACTFACTORY_H
#include <memory>

namespace logic {
class Fruit;
class Coin;
class Ghost;
class PacMan;
class Wall;

class AbstractFactory {
public:
    virtual std::shared_ptr<PacMan> createPacMan(float x, float y) = 0;

    virtual std::shared_ptr<Ghost> createGhost(float x, float y) = 0;

    virtual std::shared_ptr<Coin> createCoin(float x, float y) = 0;

    virtual std::shared_ptr<Fruit> createFruit(float x, float y) = 0;

    virtual std::shared_ptr<Wall> createWall(float x, float y) = 0;

    // virtual std::shared_ptr<Gate> createGate(float current_x, float current_y)= 0;

    virtual ~AbstractFactory() = default;
};
} // namespace logic

#endif // PACMANPROJECT_ABSTRACTFACTORY_H
