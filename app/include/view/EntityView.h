//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_ENTITYVIEW_H
#define PACMANPROJECT_ENTITYVIEW_H
#include <memory>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "entities/MovableEntityModel.h"
#include "patterns/Observer.h"


namespace sf {
    class RenderWindow;
}

namespace logic {
    class EntityModel;
}

namespace representation {
    class Camera;

    class EntityView : public logic::Observer {
    protected:
        // Pointer to logic entity → for position, direction, state.
        // It is a shared_ptr because it is owned by both logic::World
        // and logic::EntityView
        const std::shared_ptr<logic::EntityModel> _model;

        // Reference to the sprite sheet from ResourceManager
        const sf::Texture &_texture;

        // Object that is drawn on screen.
        sf::Sprite _sprite;

        sf::Vector2f _screen_position;

        // Animation frames [Cut rectangles inside the sheet (mouth open, mouth closed, etc.)]
        std::vector<sf::IntRect> _frames;
        // Which animation frame to display
        uint8_t _current_frame = 0;
        // Seconds per frame
        float _frame_duration = 0.1f;

        float _animation_timer = 0.f;

        const Camera &_camera;


        void setFrames(const std::vector<sf::IntRect> &frames);

    public:
        explicit EntityView(const std::shared_ptr<logic::EntityModel> &model, const sf::Texture &tex,
                            const Camera &camera);

        ~EntityView() override;

        // Update animation + screen_position
        virtual void update(float dt);

        virtual void render(sf::RenderWindow &window) const;
    };
}


#endif //PACMANPROJECT_ENTITYVIEW_H
