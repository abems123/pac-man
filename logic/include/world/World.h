//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WORLD_H
#define PACMANPROJECT_WORLD_H
#include <memory>
#include <vector>

#include "TileMap.h"


class Stopwatch;
class Score;
class Fruit;
class Coin;
class Ghost;
class PacManModel;

class World {
    std::vector<std::unique_ptr<Ghost> > ghosts;
    std::vector<std::unique_ptr<Coin> > coins;
    std::vector<std::unique_ptr<Fruit> > fruits;
    std::unique_ptr<PacManModel> pacman;

    TileMap tilemap;
    Score& score;
    Stopwatch& stopwatch;

public:
    World() = default;
};


#endif //PACMANPROJECT_WORLD_H
