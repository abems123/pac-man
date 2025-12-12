//
// Created by abdellah on 12/11/25.
//


#include "world/TileMap.h"

#include <string>

#include "utils/FileReader.h"


namespace logic {
    TileMap::TileMap(const std::string &file) {
        auto maze = FileReader::getFileLines(file);

        for (auto row: maze) {
            for (auto t: row) {
                tiles.push_back({});
                switch (t) {
                    case '#':
                        tiles.back().push_back(TileType::Wall);
                        break;
                }
            }
        }
    }
}
