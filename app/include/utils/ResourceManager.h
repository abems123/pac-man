//
// Created by abdellah on 12/13/25.
//

#ifndef PACMANPROJECT_RESOURCEMANAGER_H
#define PACMANPROJECT_RESOURCEMANAGER_H
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <unordered_map>

namespace representation {

/** Fonts available in the project (mapped to files in assets/fonts). */
enum class Font { Crackman, SuperMeatBall, PressStart2P, VT323, PixelifySans };

/**
 * @brief Loads and owns shared resources like fonts, textures, and the level map.
 * Singleton so everything in the app uses the same loaded SFML assets
 */
class ResourceManager {
    /** Loaded fonts indexed by our Font enum. */
    std::unordered_map<Font, sf::Font> _fonts;

    /** Main sprite sheet texture (assets/sprites/sprite.png). */
    sf::Texture _texture;

    /** Private constructor: singleton. */
    ResourceManager();

public:
    /**
     * @brief Access the single ResourceManager instance.
     * @return Global ResourceManager.
     */
    static ResourceManager& instance();

    /**
     * @brief Load one font from disk and store it under a Font key.
     * @param font Which enum key to store the font under.
     * @param path Path to the font file (ttf/otf).
     */
    void loadFont(Font font, const std::string& path);

    /**
     * @brief Get a previously loaded font.
     * @param type Font key.
     * @return Reference to the SFML font (must have been loaded).
     */
    const sf::Font& getFont(Font type) const;

    /**
     * @brief Get the main sprite sheet texture.
     * @return Reference to the loaded texture.
     */
    const sf::Texture& getTexture() const;

    /**
     * @brief Load the map text file into a vector of strings (one row per line).
     * @return The map layout.
     */
    static std::vector<std::string> getMap();
};
} // namespace representation

#endif // PACMANPROJECT_RESOURCEMANAGER_H
