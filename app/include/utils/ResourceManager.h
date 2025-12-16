//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_RESOURCEMANAGER_H
#define PACMANPROJECT_RESOURCEMANAGER_H
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include <unordered_map>


namespace representation {
    enum class Font {
        Main_Font,
        Title_Font,
        Score_Font
    };

    class ResourceManager {
        std::unordered_map<Font, sf::Font> _fonts;
        sf::Texture _texture;

        ResourceManager();

    public:
        static ResourceManager& instance();

        const sf::Font &getFont(Font type) const;

        const sf::Texture &getTexture() const;

        static const std::vector<std::string> getMap();
    };
}

#endif //PACMANPROJECT_RESOURCEMANAGER_H
