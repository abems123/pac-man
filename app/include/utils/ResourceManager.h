//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_RESOURCEMANAGER_H
#define PACMANPROJECT_RESOURCEMANAGER_H
#include <SFML/Graphics.hpp>
#include <unordered_map>



namespace representation {
    enum class Font {
        Main_Font,
        Title_Font,
        Score_Font
    };

    class ResourceManager {
    public:
        const sf::Font &getFont(Font type);

    private:
        std::unordered_map<Font, sf::Font> fonts;
        bool loaded = false;

        void loadResources(); // load all fonts once
    };
}

#endif //PACMANPROJECT_RESOURCEMANAGER_H
