//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WORLD_H
#define PACMANPROJECT_WORLD_H
#include <memory>
#include <vector>

#include "entities/EntityModel.h"
#include "entities/PacMan.h"

#include <map>
#include <utils/ResourceManager.h>

namespace representation {
class EntityView;
}

namespace logic {
class AbstractFactory;
class Event;
class EntityModel;
class Stopwatch;
class Score;
class Fruit;
class Coin;
class Ghost;
class Wall;

class World {
    std::vector<std::shared_ptr<Wall>> _walls{};
    std::vector<std::shared_ptr<Ghost>> _ghosts{};
    std::vector<std::shared_ptr<Coin>> _coins{};
    std::vector<std::shared_ptr<Fruit>> _fruits{};
    std::shared_ptr<PacMan> _pacman{};

    std::shared_ptr<Score> _score;

    std::shared_ptr<AbstractFactory> _factory;

    std::vector<std::string> map;
    const int n;
    const int m;

    // =========== Calculating models Size in the normalized coordinate system [START] ===========
    float _model_width = 0.f;
    float _model_height = 0.f;
    // =========== Calculating models Size in the normalized coordinate system [END] ===========

    std::vector<std::vector<std::shared_ptr<Wall>>> wall_at;

public:
    explicit World(const std::shared_ptr<AbstractFactory>& factory);

    void movePacMan(Direction direction, bool change_dir = true) const;

    void update(float deltaTime);

    bool intersects(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const;

    bool wouldCollide(float nextX, float nextY) const;

    bool collides(const EntityModel* a, const EntityModel* b, double horizontalDistance, double verticalDistance) const;

    void coinEaten(std::shared_ptr<Coin> coin);

    bool isWall(Direction direction) const;
};
} // namespace logic

#endif // PACMANPROJECT_WORLD_H
