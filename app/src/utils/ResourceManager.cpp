//
// Created by abdellah on 12/13/25.
//

#include "../../include/utils/ResourceManager.h"

#include <SFML/Graphics/Font.hpp>
#include <stdexcept>

#include "utils/FileManager.h"

namespace representation {
ResourceManager::ResourceManager() {
    loadFont(Font::Crackman, "../../assets/fonts/crackman.otf");
    loadFont(Font::SuperMeatBall, "../../assets/fonts/super-meatball.ttf");
    loadFont(Font::PressStart2P, "../../assets/fonts/pressstart2p.ttf");
    loadFont(Font::VT323, "../../assets/fonts/vt323-regular.ttf");
    loadFont(Font::PixelifySans, "../../assets/fonts/pixelify-sans.ttf");

    if (!_texture.loadFromFile("../../assets/sprites/sprite.png")) {
        throw std::runtime_error("Failed to load sprite.png");
    }
}

ResourceManager& ResourceManager::instance() {
    static ResourceManager inst;
    return inst;
}

void ResourceManager::loadFont(const Font font, const std::string& path) {
    sf::Font _f;
    if (!_f.loadFromFile(path)) {
        throw std::runtime_error("Failed to load super-meatball.ttf");
    }
    _fonts[font] = _f;
}

const sf::Font& ResourceManager::getFont(const Font type) const { return _fonts.at(type); }

const sf::Texture& ResourceManager::getTexture() const { return _texture; }

std::vector<std::string> ResourceManager::getMap() { return FileReader::getFileLines("../../assets/maps/map.txt"); }
} // namespace representation
