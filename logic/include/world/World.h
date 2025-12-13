//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WORLD_H
#define PACMANPROJECT_WORLD_H
#include <memory>
#include <vector>

#include "TileMap.h"


class Direction;

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
    class PacManModel;

    class World {

        std::vector<std::unique_ptr<Ghost> > ghosts{};
        std::vector<std::unique_ptr<Coin> > coins{};
        std::vector<std::unique_ptr<Fruit> > fruits{};
        std::unique_ptr<PacManModel> pacman{};

        TileMap tilemap;
        Score &score;
        Stopwatch &stopwatch;

    public:
        World();
        World(Stopwatch& stopwatch, Score& score, AbstractFactory &factory, int level);

        void movePacMan(Direction direction);

        void update(float deltaTime);

        void reset();

        bool collides(EntityModel *a, EntityModel *b);

        void removeCoin(Coin *c);

        void removeFruit(Fruit *f);

        bool isWall(Position position);

        std::pmr::vector<Direction> getViableDirections();

        void notifyScore(Event event);
    };
}


#endif //PACMANPROJECT_WORLD_H
