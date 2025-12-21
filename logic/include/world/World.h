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
#include <SFML/System/Vector2.hpp>
#include <utils/ResourceManager.h>

class Collectable;

namespace representation {
    class EntityView;
}

namespace logic {
    class LockedGhost;
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
        std::vector<std::shared_ptr<Wall> > _walls{};
        std::vector<std::shared_ptr<Collectable> > _collectables{};
        std::shared_ptr<PacMan> _pacman{};

        std::shared_ptr<Ghost> _locked_ghost;
        std::vector<std::shared_ptr<Ghost>> _predictive_ghost;
        std::shared_ptr<Ghost> _direct_ghost;


        std::unique_ptr<Score> _score;

        std::shared_ptr<AbstractFactory> _factory;

        std::vector<std::string> map;
        const int n;
        const int m;

        // =========== Calculating models Size in the normalized coordinate system [START] ===========
        float _model_width = 0.f;
        float _model_height = 0.f;
        // =========== Calculating models Size in the normalized coordinate system [END] ===========

        std::vector<std::vector<std::shared_ptr<Wall> > > wall_at;

        int _lives_left = 3;

    public:
        void setWallsTypes() const;

        void parseMap();

        void startGhosts();

        bool isIntersection(int r, int c, Direction currentDir) const;

        [[nodiscard]] Direction oppositeOf(Direction d) const;

        void updateGhost(Ghost &g);


        explicit World(const std::shared_ptr<AbstractFactory> &factory);

        void movePacMan(Direction direction);

        void collect();

        std::vector<Direction> getViableDirections(int c, int r) const;

        [[nodiscard]] std::vector<Direction> getViableDirections(const std::shared_ptr<MovableEntityModel> & entityModel) const;

        void update();


        [[nodiscard]] bool intersects(float ax, float ay, float aw, float ah, float bx, float by, float bw,
                                      float bh) const;


        void eat(const std::shared_ptr<Collectable> &collectable);


        [[nodiscard]] Score *getScore() const { return _score.get(); }

        [[nodiscard]] int colFromX(float x) const;

        [[nodiscard]] int rowFromY(float y) const;

        [[nodiscard]] float tileLeftX(int col) const;

        [[nodiscard]] bool isWallCell(int r, int c) const;

        [[nodiscard]] bool collides(const EntityModel *m1, const EntityModel *m2) const;
    };
} // namespace logic

#endif // PACMANPROJECT_WORLD_H
