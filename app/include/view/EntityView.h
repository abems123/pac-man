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

#include "camera/Camera.h"
#include "entities/MovableEntityModel.h"
#include "patterns/Observer.h"
#include "utils/Constants.h"
#include "utils/ResourceManager.h"


namespace sf {
    class RenderWindow;
}

namespace logic {
    class EntityModel;
}

namespace representation {
    class Sprite;
    class Camera;

    class EntityView : public logic::Observer {
    protected:
        // Pointer to logic entity → for position, direction, state.
        // It is a shared_ptr because it is owned by both logic::World
        // and logic::EntityView
        const std::weak_ptr<logic::EntityModel> _model;

        // Object that is drawn on screen.
        sf::Sprite _sprite;

        std::vector<sf::Vector2i> _sprites_sizes;

        sf::Vector2f _position;

        std::vector<sf::IntRect> _frames;

    public:
        explicit EntityView(const std::shared_ptr<logic::EntityModel> &model);

        EntityView(const std::shared_ptr<logic::EntityModel> &model, int column, int row, int number_of_frames);

        virtual void update(float dt);
        void updatePosition();

        virtual void render(sf::RenderWindow &window) const;

        bool modelExpired() const { return _model.expired(); }
    };
}


#endif //PACMANPROJECT_ENTITYVIEW_H
