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

/**
 * @brief Concrete implementation of the AbstractFactory used by the game (app/representation side).
 *
 * This factory creates the *logic* entities (PacMan, Ghost, Coin, Fruit, Wall) and, at the same time,
 * also creates and registers the corresponding *views* (EntityView) so they can be rendered.
 *
 * The logic layer only depends on AbstractFactory, while the representation layer chooses to pass this
 * ConcreteFactory to connect models with their SFML views.
 */
class ConcreteFactory final : public logic::AbstractFactory {
    /**
     * @brief Non-owning pointer to the container that stores all active views.
     *
     * ConcreteFactory pushes freshly created views into this vector so the game can update/render them.
     * The vector itself is owned elsewhere (typically by Game / LevelState).
     */
    std::vector<std::shared_ptr<EntityView>>* _views;

    /**
     * @brief Counter used to track how many ghosts were created so far.
     *
     * Useful when ghosts need different view types/skins/behaviours depending on their spawn order
     * (e.g., different colors or AI types per ghost).
     */
    int _created_ghosts = 0;

public:
    /**
     * @brief Construct a factory that will register views into the provided view list.
     *
     * @param views Pointer to the view container where created EntityView instances must be stored.
     *              Must remain valid for the entire lifetime of this factory.
     */
    explicit ConcreteFactory(std::vector<std::shared_ptr<EntityView>>* views);


    ~ConcreteFactory() override;

    /**
     * @brief Create a Pac-Man model at the given world position and register its view.
     *
     * @param x World X coordinate (model space).
     * @param y World Y coordinate (model space).
     * @return Shared pointer to the created PacMan model.
     */
    std::shared_ptr<logic::PacMan> createPacMan(float x, float y) override;

    /**
     * @brief Create a Ghost model at the given world position and register its view.
     *
     * @param x World X coordinate (model space).
     * @param y World Y coordinate (model space).
     * @return Shared pointer to the created Ghost model.
     */
    std::shared_ptr<logic::Ghost> createGhost(float x, float y) override;

    /**
     * @brief Create a Coin model at the given world position and register its view.
     *
     * @param x World X coordinate (model space).
     * @param y World Y coordinate (model space).
     * @return Shared pointer to the created Coin model.
     */
    std::shared_ptr<logic::Coin> createCoin(float x, float y) override;

    /**
     * @brief Create a Fruit model at the given world position and register its view.
     *
     * @param x World X coordinate (model space).
     * @param y World Y coordinate (model space).
     * @return Shared pointer to the created Fruit model.
     */
    std::shared_ptr<logic::Fruit> createFruit(float x, float y) override;

    /**
     * @brief Create a Wall model at the given world position and register its view.
     *
     * @param x World X coordinate (model space).
     * @param y World Y coordinate (model space).
     * @return Shared pointer to the created Wall model.
     */
    std::shared_ptr<logic::Wall> createWall(float x, float y, bool isGate) override;
};
} // namespace representation

#endif // PACMANPROJECT_CONCRETEFACTORY_H
