//
// Created by abdellah on 12/17/25.
//

#ifndef PACMANPROJECT_ANIMATIONMANAGER_H
#define PACMANPROJECT_ANIMATIONMANAGER_H
#include <SFML/Graphics/Rect.hpp>
#include <vector>

namespace representation {
class AnimationManager {
public:
    AnimationManager() = default;

    /**
     * @brief Append a vertical sequence of frames from the sprite sheet into a vector.
     *
     * The frames are taken from a fixed sprite-sheet column, starting at @p row and
     * going downwards for @p frames_number frames (so it uses rows: row, row+1, ...).
     *
     * @param column        Sprite-sheet column index where the frames are located.
     * @param row           Starting sprite-sheet row index (top frame).
     * @param frames_number How many consecutive frames to generate.
     * @param frames        Output vector where the generated sf::IntRect frames are added to.
     *
     */
    static void setFrames(int column, int row, int frames_number, std::vector<sf::IntRect>& frames);
};
} // namespace representation

#endif // PACMANPROJECT_ANIMATIONMANAGER_H
