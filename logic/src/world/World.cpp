//
// Created by abdellah on 12/11/25.
//

#include "../include/world/World.h"

#include <iostream>

#include "entities/PacMan.h"
#include "entities/Wall.h"
#include "patterns/AbstractFactory.h"
#include "score/Score.h"
#include "utils/FileReader.h"


namespace logic {

    World::World(const std::shared_ptr<AbstractFactory> &factory) : _score(std::make_shared<Score>()), _factory(factory) {

        // std::cout << "============================================" << std::endl;
        // std::cout << "Tile Height in World = " << getTileHeight() << std::endl;
        // std::cout << "Tile Width in World = " << getTileWidth() << std::endl;
        // std::cout << "============================================" << std::endl;

        for (float i = -0.8; i <= 0.8; i+=.1) {
            auto wall = _factory->createWall(i, i);
            _walls.push_back(wall);
        }

        /*
        for (int i = 0, n = map.size(); i < n; ++i) {
            for (int j = 0, m = map[i].length(); j < m; ++j) {
                const float current_x = 2.0 * j / m - 1; // (static_cast<float>(j) - m / 2.0) / (m / 2.0);
                const float current_y = 2.0 * i / n - 1; // (static_cast<float>(i) - n / 2.0) / (n / 2.0);

                auto current_c = map[i][j];

                if (current_c == '#') {
                    auto wall = _factory->createWall(current_x, current_y);
                    _walls.push_back(wall);
                } else if (current_c == '*') {
                    auto coin = _factory->createCoin(current_x, current_y);
                    _coins.push_back(coin);
                } else if (current_c == '&') {
                    auto barrier = _factory->createBarrier(current_x, current_y);
                    _barriers.push_back(barrier);
                } else if (current_c == 'P') {
                    _pacman = _factory->createPacMan(current_x, current_y);
                } else if (current_c == '$') {
                    auto fruit = _factory->createFruit(current_x, current_y);
                    _fruits.push_back(fruit);
                } else if (current_c == '@') {
                    auto ghost = _factory->createDirectChaseGhost(current_x, current_y);
                    _ghosts.push_back(ghost);
                } else if (current_c == 'E') {
                }

            }
        }*/
    }

    void World::movePacMan(const Direction direction) const {
        switch (direction) {
            case Direction::Down:
                _pacman->move(0, -_pacman->getSpeed() * Stopwatch::getInstance().dt());
                _pacman->setDirection(Direction::Down);
                break;
            case Direction::Up:
                _pacman->move(0, _pacman->getSpeed() * Stopwatch::getInstance().dt());
                _pacman->setDirection(Direction::Up);
                break;
            case Direction::Right:
                _pacman->move(_pacman->getSpeed() * Stopwatch::getInstance().dt(), 0);
                _pacman->setDirection(Direction::Right);
                break;
            case Direction::Left:
                _pacman->move(-_pacman->getSpeed() * Stopwatch::getInstance().dt(), 0);
                _pacman->setDirection(Direction::Left);
                break;
            case Direction::None:
                break;
        }
    }

}
