//
// Created by abdellah on 12/12/25.
//

#include "../../include/view/PacMan.h"

#include <iostream>

#include "../../logic/include/entities/EntityModel.h"
#include "../../logic/include/entities/PacMan.h"
#include "events/Event.h"


namespace representation {
    PacMan::PacMan(const std::shared_ptr<logic::EntityModel> &pm, const sf::Texture &texture,
                   const Camera &camera) : EntityView(pm,
                                                      texture,
                                                      camera) {

        int sprite_size = 35;
        int space_between_sprites = 15;
        int start_left = 1;
        int start_top = 4;

        // Each frame is a sub-rectangle of the sprite sheet
        const std::vector<sf::IntRect> frames = {
            {sf::IntRect(start_left + 17 * (sprite_size + space_between_sprites), start_top, sprite_size, sprite_size)}, // mouth closed
            {sf::IntRect(start_left + 17 * (sprite_size + space_between_sprites), start_top + 1 * (space_between_sprites + sprite_size), sprite_size, sprite_size)}, // mouth half open
            {sf::IntRect(start_left + 17 * (sprite_size + space_between_sprites), start_top + 2 * (space_between_sprites + sprite_size), sprite_size, sprite_size)} // mouth fully open
        };

        setFrames(frames);

        _frame_duration = 0.5f;
    }

    void PacMan::update(const float dt) {
        // This is used only to update the animation, the movements and direction changing is handeled by onNotify
        EntityView::update(dt);
    }

    void PacMan::onNotify(const logic::EventType &event) {
        switch (event) {
            case logic::EventType::DirectionChanged:
                // updateDirectionFrames();
                std::cout << "Direction Changed" << std::endl;
                break;

            case logic::EventType::Moved:
                // updateScreenPosition();
                std::cout << "Pacman Moved" << std::endl;
                break;

                /*
            case logic::EventType::PowerChanged:
                updatePowerColor();
                break;

            case logic::EventType::Died:
                startDeathAnimation();
                break;
                */
        }
    }

    void PacMan::render(sf::RenderWindow &window) const {
        EntityView::render(window);
    }
}
