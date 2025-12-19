//
// Created by Abdellah on 12/18/2025.
//

#include "../../logic/include/utils/ResourceManager.h"

namespace logic {
std::vector<std::string> ResourceManager::getMap() {
    return FileReader::getFileLines("../../assets/maps/map.txt");
}
} // namespace logic
