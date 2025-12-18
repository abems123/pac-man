//
// Created by abdellah on 12/17/25.
//

#ifndef PACMANPROJECT_ANIMATIONMANAGER_H
#define PACMANPROJECT_ANIMATIONMANAGER_H
#include <vector>
#include <SFML/Graphics/Rect.hpp>


namespace representation {
    class AnimationManager {
    public:
        AnimationManager() = default;

        static void setFrames(int column, int row, int frames_number, std::vector<sf::IntRect> &frames);
    };
}


#endif //PACMANPROJECT_ANIMATIONMANAGER_H
