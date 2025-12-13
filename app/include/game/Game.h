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
        sf::RenderWindow window;

        // Manages all states (Menu, Level, Pause, Victory)
        StateManager stateManager;

        // Used by views to project world coords → pixels
        Camera camera;

        // Representation-side factory (creates model+view pairs)
        ConcreteFactory factory;

        // Textures, fonts (optional but common)
        ResourceManager resources;

        // For delta-time (representation timing)
        logic::Stopwatch stopwatch;

        void processEvents();

        void handleInput();

        void update(float dt);

        void render();

    public:
        Game();

        void run();

        sf::RenderWindow &getWindow() { return window; }
        ResourceManager getResources() const { return resources; }
    };
}


#endif //PACMANPROJECT_GAME_H
