//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_ENTITYMODEL_H
#define PACMANPROJECT_ENTITYMODEL_H
#include "patterns/Subject.h"

namespace logic {
enum class GhostMode { Center, Chase, Fear, Dead };

enum class Direction { Up, Down, Right, Left, None };

enum class EntityType { Pacman, Wall, Coin, Fruit, LockedGhost, FollowerGhost, LookaheadChaseGhost, Unknown };

class EntityModel : public Subject {
protected:
    /** @brief Concrete type of this entity (Pacman, Wall, Ghost, ...). */
    EntityType _type;
    /** @brief X position in world coordinates. */
    float _x, _y;

public:
    /**
     * @brief Constructs a model with a position and entity type.
     * @param x Initial x position in world coordinates.
     * @param y Initial y position in world coordinates.
     * @param type Entity kind represented by this model.
     */
    EntityModel(const float x, const float y, const EntityType type) : _type(type), _x(x), _y(y) {}

    /**
     * @brief Sets the model's world position.
     * @param x New x position in world coordinates.
     * @param y New y position in world coordinates.
     * @return void
     */
    void setPosition(float x, float y);

    /**
     * @brief Gets the model's world position.
     * @return Pair {x, y} in world coordinates.
     */
    std::pair<float, float> getPosition() const { return {_x, _y}; }

    /**
     * @brief Returns what kind of entity this model represents.
     * @return Entity type stored in this model.
     */
    [[nodiscard]] EntityType getType() const { return _type; }
};
} // namespace logic

#endif // PACMANPROJECT_ENTITYMODEL_H
