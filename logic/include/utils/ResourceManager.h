//
// Created by Abdellah on 12/18/2025.
//

#ifndef RESOURCEMANAGER_MODEL_H
#define RESOURCEMANAGER_MODEL_H

#include <string>
#include <utils/FileReader.h>
#include <vector>

namespace logic {
class ResourceManager {
public:
    static std::vector<std::string> getMap();
};
} // namespace logic

#endif // RESOURCEMANAGER_MODEL_H
