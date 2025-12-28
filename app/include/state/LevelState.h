//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_LEVELSTATE_H
#define PACMANPROJECT_LEVELSTATE_H
#include <memory>

#include "State.h"
#include "game/Game.h"
#include "world/World.h"

namespace representation {
class EntityView;

class LevelState : public State {
    std::unique_ptr<logic::World> _world;

    std::vector<std::shared_ptr<EntityView>> _views;

    // Representation-side factory (creates both the model+view)
    std::shared_ptr<logic::AbstractFactory> _factory;

    // Score text
    sf::Text score;

    sf::Text _pac_loc;

public:
    LevelState(StateManager& manager, Game& game, int level);

    void handleInput() override;

    void update(float dt) override;

    void render(sf::RenderWindow& window) override;
};
} // namespace representation

#endif // PACMANPROJECT_LEVELSTATE_H
