//
// Created by abdellah on 12/13/25.
//

#include "../../include/utils/ResourceManager.h"

#include <stdexcept>
#include <SFML/Graphics/Font.hpp>

namespace representation {

    void ResourceManager::loadResources() {

        // Load main font
        sf::Font main;
        if (!main.loadFromFile("../../assets/fonts/crackman.otf")) {
            throw std::runtime_error("Failed to load crackman.otf");
        }
        fonts[Font::Main_Font] = std::move(main);

        // Example: more fonts later
        // sf::Font title;
        // title.loadFromFile("../assets/fonts/anotherFont.otf");
        // fonts[Font::Title_Font] = std::move(title);
        //
        // sf::Font score;
        // score.loadFromFile("../assets/fonts/scoreFont.otf");
        // fonts[Font::Score_Font] = std::move(score);

        loaded = true;
    }

    const sf::Font& ResourceManager::getFont(const Font type) {

        if (!loaded) {
            loadResources();
        }

        return fonts.at(type);
    }

}
