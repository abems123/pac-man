//
// Created by abdellah on 12/12/25.
//

#ifndef PACMANPROJECT_ENTITYVIEW_H
#define PACMANPROJECT_ENTITYVIEW_H
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <vector>

#include "camera/Camera.h"
#include "entities/MovableEntityModel.h"
#include "patterns/Observer.h"
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
    /**
     * @brief Non-owning link to the model represented by this view.
     *
     * The World owns the model (shared_ptr). The view only *observes* it,
     * so we store a weak_ptr to avoid extending the model's lifetime and to prevent
     * ownership cycles (e.g., model <-> view via observers).
     *
     * Use lock() to access it safely; it may expire when the model is removed (coin eaten, etc.).
     */
    const std::weak_ptr<logic::EntityModel> _model;

    /** @brief SFML sprite used to draw this entity on screen. */
    sf::Sprite _sprite;

    /**
     * @brief Texture rectangles for the current animation frames.
     *
     * Each rect selects a sub-region of the sprite sheet to display.
     */
    std::vector<sf::IntRect> _frames;

public:
    /**
     * @brief Create a view for a model using default sprite/animation settings.
     *
     * @param model Shared ownership handle to the model; stored as a weak_ptr internally.
     */
    explicit EntityView(const std::shared_ptr<logic::EntityModel>& model);

    /**
     * @brief Create a view for a model with an initial sprite-sheet region and animation setup.
     *
     * @param model Shared ownership handle to the model; stored as a weak_ptr internally.
     * @param column Sprite-sheet column (tile index) where the animation starts.
     * @param row Sprite-sheet row (tile index) where the animation starts.
     * @param number_of_frames How many frames belong to this animation.
     */
    EntityView(const std::shared_ptr<logic::EntityModel>& model, int column, int row, int number_of_frames);

    /**
     * @brief Update animation state and refresh cached render data.
     *
     * @param dt Delta time in seconds since last frame.
     */
    virtual void update(float dt);

    /**
     * @brief Update sprite position/scale from the model using the current camera transform.
     *
     * Call this when the camera changes, the window resizes, or the model moved.
     */
    virtual void updatePosition();

    /**
     * @brief Draw this entity to the given window.
     *
     * @param window Target window to render into.
     */
    virtual void render(sf::RenderWindow& window) const;

    /**
     * @brief Get the logical entity type of the model behind this view.
     *
     * @return Entity type (PacMan, Ghost, Coin, Wall, ...).
     */
    [[nodiscard]] logic::EntityType getModelType() const;

    /**
     * @brief Check whether the underlying model no longer exists.
     *
     * Useful for cleaning up views when the model was removed from the world.
     *
     * @return True if the model has been destroyed, false otherwise.
     */
    bool modelExpired() const { return _model.expired(); }

};
} // namespace representation

#endif // PACMANPROJECT_ENTITYVIEW_H
