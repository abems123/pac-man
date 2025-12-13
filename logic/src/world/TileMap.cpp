//
// Created by abdellah on 12/11/25.
//


#include "world/TileMap.h"

#include <string>

#include "utils/FileReader.h"


namespace logic {
    TileMap::TileMap(const std::string &file) {
        for (const auto maze = FileReader::getFileLines(file); auto row: maze) {
            for (const auto t: row) {
                tiles.push_back({});
                switch (t) {
                    case '#':
                        tiles.back().push_back(TileType::Wall);
                        break;
                    case '*':
                        tiles.back().push_back(TileType::Coin);
                        break;
                    case '&':
                        tiles.back().push_back(TileType::Barrier);
                        break;
                    case 'P':
                        tiles.back().push_back(TileType::PacMan);
                        break;
                    case '$':
                        tiles.back().push_back(TileType::Fruit);
                        break;
                    case '@':
                        tiles.back().push_back(TileType::Ghost);
                        break;
                    case 'E':
                        tiles.back().push_back(TileType::Empty);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
