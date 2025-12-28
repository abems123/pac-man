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

/**
 * @brief Abstract Factory interface used to spawn game entities.
 *
 * World use this interface to create entities without
 * depending on concrete classes. This keeps the logic layer decoupled from
 * the representation layer (and makes it easier to swap implementations,
 * e.g., different sprites/behaviors or test doubles).
 *
 * All creation functions receive the spawn position in world coordinates
 * (top-left of the tile, in the [-1, 1[ coordinate system).
 */
class AbstractFactory {
public:
    /**
     * @brief Create a Pac-Man entity at a given position.
     * @param x World x coordinate (tile top-left).
     * @param y World y coordinate (tile top-left).
     * @return Shared pointer to the created PacMan model.
     */
    virtual std::shared_ptr<PacMan> createPacMan(float x, float y) = 0;

    /**
     * @brief Create a Ghost entity at a given position.
     * @param x World x coordinate (tile top-left).
     * @param y World y coordinate (tile top-left).
     * @return Shared pointer to the created Ghost model.
     */
    virtual std::shared_ptr<Ghost> createGhost(float x, float y) = 0;

    /**
     * @brief Create a Coin collectable at a given position.
     * @param x World x coordinate (tile top-left).
     * @param y World y coordinate (tile top-left).
     * @return Shared pointer to the created Coin model.
     */
    virtual std::shared_ptr<Coin> createCoin(float x, float y) = 0;

    /**
     * @brief Create a Fruit collectable at a given position.
     * @param x World x coordinate (tile top-left).
     * @param y World y coordinate (tile top-left).
     * @return Shared pointer to the created Fruit model.
     */
    virtual std::shared_ptr<Fruit> createFruit(float x, float y) = 0;

    /**
     * @brief Create a Wall entity at a given position.
     * @param x World x coordinate (tile top-left).
     * @param y World y coordinate (tile top-left).
     * @param isGate True if this is a gate.
     * @return Shared pointer to the created Wall model.
     */
    virtual std::shared_ptr<Wall> createWall(float x, float y, bool isGate) = 0;

    virtual ~AbstractFactory() = default;
};

} // namespace logic

#endif // PACMANPROJECT_ABSTRACTFACTORY_H
