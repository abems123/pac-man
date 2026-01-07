//
// Created by abdellah on 12/13/25.
//

#pragma once

#include "EntityModel.h"

#include <set>

namespace logic {
class World;
class MovableEntityModel : public EntityModel {
protected:
    /** @brief Current movement speed in world units per second. */
    float _speed = 0.2f;
    /** @brief Current movement direction. */
    Direction _direction = Direction::Right;

    /** @brief Non-owning pointer to the World used for collision/queries. */
    World* _world = nullptr;

    /** @brief Cached movement permissions for each direction (set by collision logic). */
    bool _can_move_right = false;
    /** @brief Cached movement permissions for each direction (set by collision logic). */
    bool _can_move_left = false;
    /** @brief Cached movement permissions for each direction (set by collision logic). */
    bool _can_move_down = false;
    /** @brief Cached movement permissions for each direction (set by collision logic). */
    bool _can_move_up = false;

public:
    /**
     * @brief Constructs a movable entity model with speed and type.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     * @param type Entity kind represented by this model.
     * @param speed Initial movement speed.
     */
    MovableEntityModel(float x, float y, EntityType type, float speed);

    /**
     * @brief Gets the current movement speed.
     * @return Speed in world units per second.
     */
    [[nodiscard]] float getSpeed() const { return _speed; }

    /**
     * @brief Sets the movement speed.
     * @param speed New speed in world units per second.
     * @return void
     */
    void setSpeed(const float speed) { this->_speed = speed; }

    /**
     * @brief Gets the current movement direction.
     * @return Current direction.
     */
    [[nodiscard]] Direction getDirection() const { return _direction; };

    /**
     * @brief Sets the current movement direction.
     * @param direction New direction.
     * @return void
     */
    void setDirection(Direction direction);

    /**
     * @brief Updates the entity movement for the current frame.
     * @param world
     * @param dt Delta time in seconds.
     * @return void
     */
    virtual void update(World* world, float dt);

    /**
     * @brief Sets whether movement is allowed in a specific direction.
     * @param dir Direction to configure.
     * @param can_move True if movement in this direction is allowed.
     * @return void
     */
    void setMove(Direction dir, bool can_move);


    /**
     * @brief Shared grid clamp for Pac-Man + ghosts.
     *
     * World decides what's "viable". This just applies the flags + clamps.
     *
     * @param world World for x/y conversions.
     * @param row Current row.
     * @param col Current col.
     * @param viable Allowed directions to enter from this cell.
     * @param dt Delta time.
     * @param model_w Tile width in world units.
     * @param model_h Tile height in world units.
     * @param center_eps_ratio How strict the "at center" check is.
     * @return True if entity is close enough to cell center (good moment to turn).
     */
    bool applyGridConstraints(const World& world,
                              int row,
                              int col,
                              const std::set<Direction>& viable,
                              float dt,
                              float model_w,
                              float model_h,
                              float center_eps_ratio = 0.03f);
};
} // namespace logic
