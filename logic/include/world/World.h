//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WORLD_H
#define PACMANPROJECT_WORLD_H
#include <memory>
#include <vector>

#include "../../../app/include/utils/ResourceManager.h"
#include "entities/EntityModel.h"
#include "entities/PacMan.h"


namespace logic {
    class AbstractFactory;
    class Event;
    struct Position;
    class EntityModel;
    class Stopwatch;
    class Score;
    class Fruit;
    class Coin;
    class Ghost;
    class Wall;

    using TileMap = std::vector<std::string>;

    class World {
        std::vector<std::shared_ptr<Wall> > _walls{};
        std::vector<std::shared_ptr<Ghost> > _ghosts{};
        std::vector<std::shared_ptr<Coin> > _coins{};
        std::vector<std::shared_ptr<Fruit> > _fruits{};
        std::shared_ptr<PacMan> _pacman{};

        // TileMap tilemap;
        std::shared_ptr<Score> _score;

        std::shared_ptr<AbstractFactory> _factory;

        // I don't need level here, because I'm gonna handel level from LevelState
        // and I'm gonna set speed and change the hardness from there
        // int level = 0;


    public:
        explicit World(const std::shared_ptr<AbstractFactory> &factory);

        void movePacMan(Direction direction) const;

        void update(float deltaTime);

        void reset();

        bool collides(EntityModel *a, EntityModel *b);

        void removeCoin(Coin *c);

        void removeFruit(Fruit *f);

        bool isWall(Position position);

        std::pmr::vector<Direction> getViableDirections();

        void notifyScore(Event event);

        static float getTileWidth();

        static float getTileHeight();

        // I want to use these methods currently for tests
        void setLevel(int level);

        void setTileMap(TileMap *tilemap);

        void setPacman(PacMan *pacman) { this->_pacman = std::make_unique<PacMan>(*pacman); }

        void addGhost(std::unique_ptr<Ghost> ghost);

        void addCoin(std::unique_ptr<Coin> coin);

        void addFruit(std::unique_ptr<Fruit> fruit);
    };
}


#endif //PACMANPROJECT_WORLD_H
