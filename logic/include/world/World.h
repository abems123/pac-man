//
// Created by abdellah on 12/11/25.
//

#ifndef PACMANPROJECT_WORLD_H
#define PACMANPROJECT_WORLD_H
#include <memory>
#include <vector>

#include "../../../app/include/utils/ResourceManager.h"
#include "entities/EntityModel.h"
#include "entities/PacMan.h"


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
        std::vector<std::shared_ptr<Wall> > _walls{};
        std::vector<std::shared_ptr<Ghost> > _ghosts{};
        std::vector<std::shared_ptr<Coin> > _coins{};
        std::vector<std::shared_ptr<Fruit> > _fruits{};
        std::shared_ptr<PacMan> _pacman{};

        std::shared_ptr<Score> _score;

        std::shared_ptr<AbstractFactory> _factory;

        double _model_width, _model_height;

    public:
        explicit World(const std::shared_ptr<AbstractFactory> &factory);

        void movePacMan(Direction direction, bool change_dir=true) const;

        void update(float deltaTime);


        bool collides(const EntityModel *a, const EntityModel *b, double horizontalDistance,
                      double verticalDistance) const;

        void coinEaten(std::shared_ptr<Coin> coin);
    };
}


#endif //PACMANPROJECT_WORLD_H
