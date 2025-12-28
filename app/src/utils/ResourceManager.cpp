//
// Created by abdellah on 12/13/25.
//

#include "../../include/utils/ResourceManager.h"

#include <SFML/Graphics/Font.hpp>
#include <stdexcept>

#include "utils/FileReader.h"

namespace representation {
ResourceManager::ResourceManager() {
    sf::Font main;
    if (!main.loadFromFile("../../assets/fonts/crackman.otf")) {
        throw std::runtime_error("Failed to load crackman.otf");
    }

    _fonts[Font::Main_Font] = std::move(main);

    if (!_texture.loadFromFile("../../assets/sprites/sprite.png")) {
        throw std::runtime_error("Failed to load sprite.png");
    }
}

ResourceManager& ResourceManager::instance() {
    static ResourceManager inst;
    return inst;
}

const sf::Font& ResourceManager::getFont(const Font type) const { return _fonts.at(type); }

const sf::Texture& ResourceManager::getTexture() const { return _texture; }

const std::vector<std::string> ResourceManager::getMap() {
    return FileReader::getFileLines("../../assets/maps/map.txt");
}
} // namespace representation
