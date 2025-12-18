//
// Created by abdellah on 12/17/25.
//

#include "../../include/utils/AnimationManager.h"

#include "utils/Constants.h"

namespace representation {
    void AnimationManager::setFrames(const int column, const int row, const int frames_number,
                                     std::vector<sf::IntRect> &frames,
                                     std::vector<std::pair<int, int> >& sprites_width) {
        if (sprites_width.empty()) {
            for (int i = 0; i < frames_number; i++) {
                sprites_width.emplace_back(Constants::SPRITE_SIZE, Constants::SPRITE_SIZE);
            }
        }
            for (int i = row; i < row + frames_number; ++i) {
                sf::IntRect frame = {
                    Constants::START_LEFT_SPACE + column * (Constants::SPACE_BETWEEN_SPRITES + Constants::SPRITE_SIZE),
                    Constants::START_TOP_SPACE + i * (Constants::SPRITE_SIZE + Constants::SPACE_BETWEEN_SPRITES),
                    Constants::SPRITE_SIZE, Constants::SPRITE_SIZE
                };

                frames.push_back(frame);
            }
    }
}
