//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_TILEMAP_H
#define PACMANPROJECT_TILEMAP_H

#include <vector>
#include <string>

namespace logic {
    enum class TileType { Wall, Empty, Coin, Fruit, Ghost, PacMan };

    class TileMap {
        std::vector<std::vector<TileType> > tiles = {};

    public:
        explicit TileMap(const std::string &file);
    };
}


#endif //PACMANPROJECT_TILEMAP_H
