//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_GAME_H
#define PACMANPROJECT_GAME_H
#include <stack>
#include <SFML/Graphics/RenderWindow.hpp>

#include "camera/Camera.h"
#include "factory/ConcreteFactory.h"
#include "state/StateManager.h"
#include "world/World.h"
#include "../../logic/include/utils/Stopwatch.h"
#include "utils/ResourceManager.h"


namespace representation {
    class StateManager;

    /** @class Game: this class is responsible for setting up anything that is not related to the core logic of the game
     ***/
    class Game {
        // Creates and owns the SFML window
        sf::RenderWindow _window;

        // Manages all states (Menu, Level, Pause, Victory)
        StateManager _stateManager;

        // Used by views to project world coords → pixels
        Camera _camera;

        // logic::TileMap map = ResourceManager::getMap();

        // Representation-side factory (creates both the model+view)
        std::shared_ptr<ConcreteFactory> _factory;


        void processEvents();

        void handleInput();

        void update(float dt);

        void render();

    public:
        Game();

        // static logic::TileMap &getMap();

        void isMapConsistent();

        void run();

        sf::RenderWindow &getWindow() { return _window; }
        ResourceManager getResources() const { return ResourceManager::instance(); }
        std::shared_ptr<ConcreteFactory> getFactory() const { return _factory; }
    };
}


#endif //PACMANPROJECT_GAME_H
