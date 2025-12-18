//
// Created by abdellah on 12/11/25.
//

#include "../include/world/World.h"

#include <iostream>

#include "entities/PacMan.h"
#include "entities/Wall.h"
#include "entities/collectable/Coin.h"
#include "patterns/AbstractFactory.h"
#include "score/Score.h"
#include "utils/FileReader.h"
#include <algorithm>

namespace logic {
    World::World(const std::shared_ptr<AbstractFactory> &factory) : _score(std::make_shared<Score>()),
                                                                    _factory(factory) {
        // =========== Calculating models Size in the normalized coordinate system [START] ===========
        _model_width = 2.0 / representation::ResourceManager::getMap().front().size();
        _model_height = 2.0 / representation::ResourceManager::getMap().size();
        // =========== Calculating models Size in the normalized coordinate system [END] ===========

        _pacman = _factory->createPacMan(0, 0);

        auto wall = _factory->createWall(0, _model_height);
        _walls.push_back(wall);


        // =========== Parsing assets/map.txt to generate the world [START] ===========
        /*for (int i = 0, n = representation::ResourceManager::getMap().size(); i < n; ++i) {
            for (int j = 0, m = representation::ResourceManager::getMap()[i].length(); j < m; ++j) {
                const float current_x = 2.0 * j / m - 1; // (static_cast<float>(j) - m / 2.0) / (m / 2.0);
                const float current_y = 2.0 * i / n - 1; // (static_cast<float>(i) - n / 2.0) / (n / 2.0);

                auto current_c = representation::ResourceManager::getMap()[i][j];

                if (current_c == '#') {
                    auto wall = _factory->createWall(current_x, current_y);
                    _walls.push_back(wall);
                } else if (current_c == '*') {
                    auto coin = _factory->createCoin(current_x, current_y);
                    _coins.push_back(coin);
                }else if (current_c == '&') {
                     auto barrier = _factory->createBarrier(current_x, current_y);
                     _barriers.push_back(barrier);
                 }else if (current_c == 'P') {
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
        // =========== Parsing assets/map.txt to generate the world [END] ===========

        // =========== Setting Pac Man Speed [START] ===========
        _pacman->setSpeed(_model_width * 2.f);
        // =========== Setting Pac Man Speed [END] ===========
    }


    void World::movePacMan(Direction direction, bool change_dir) const {
        const float dt = Stopwatch::getInstance().dt();
        const float dist = _pacman->getSpeed() * dt;

        float dx = 0.f;
        float dy = 0.f;

        switch (direction) {
            case Direction::Up: dy = -dist;
                break;
            case Direction::Down: dy = dist;
                break;
            case Direction::Left: dx = -dist;
                break;
            case Direction::Right: dx = dist;
                break;
        }

        const float nextX = _pacman->getPosition().first + dx;
        const float nextY = _pacman->getPosition().second + dy;

        // 🚫 BLOCK movement BEFORE it happens
        if (wouldCollide(nextX, nextY))
            return;

        // ✅ Safe to move
        _pacman->move(dx, dy);

        if (change_dir)
            _pacman->setDirection(direction);
    }

    void World::update(float deltaTime) {
        std::ranges::any_of(_coins, [this](const std::shared_ptr<Coin> &c) {
            bool eats = collides(_pacman.get(), c.get(), 0.03, 0.03);
            if (eats) {
                coinEaten(c);

                std::cout << std::endl << "Pacman ate a coin!" << std::endl;
                std::cout << "========== PacMan Position ==========" << std::endl;
                std::cout << _pacman->getPosition().first << ", " << _pacman->getPosition().second << std::endl;
                std::cout << "========== Coin Position ==========" << std::endl;
                std::cout << c->getPosition().first << ", " << c->getPosition().second << std::endl;
                std::cout << "============================================" << std::endl << std::endl;
            }


            return eats;
        });

        /*
        std::ranges::any_of(_walls, [this](const std::shared_ptr<Wall> &w) {
            bool touches_wall = collides(_pacman.get(), w.get(), _model_width - 0.01, _model_height - 0.01);
            if (touches_wall) {
                switch (_pacman->getDirection()) {
                    case Direction::Down:
                        movePacMan(Direction::Up, false);
                        break;
                    case Direction::Up:
                        movePacMan(Direction::Down, false);
                        break;
                    case Direction::Right:
                        movePacMan(Direction::Left, false);
                        break;
                    case Direction::Left:
                        movePacMan(Direction::Right, false);
                        break;
                }
            }
            return touches_wall;
        });*/
    }

    bool World::intersects(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const {
        return ax < bx + bw &&
               ax + aw > bx &&
               ay < by + bh &&
               ay + ah > by;
    }

    bool World::wouldCollide(float nextX, float nextY) const {
        for (const auto &wall: _walls) {
            if (intersects(
                nextX, nextY, _model_width, _model_height,
                wall->getPosition().first,
                wall->getPosition().second,
                _model_width, _model_height)) {
                std::cout << "=========== Wall Collision ==========" << std::endl;
                std::cout << "Pacman wants to go to x=" << nextX << ", y=" << nextY << ", but there is a wall in x=" <<
                        wall->getPosition().first << ", y=" << wall->getPosition().second << std::endl;
                std::cout << "=====================================" << std::endl << std::endl;
                return true;
            }
        }
        return false;
    }

    bool World::collides(const EntityModel *a, const EntityModel *b, double horizontalDistance = 0.04,
                         double verticalDistance = 0.04) const {
        float x_distance = std::abs(a->getPosition().first - b->getPosition().first);
        float y_distance = std::abs(a->getPosition().second - b->getPosition().second);

        return x_distance < horizontalDistance /*_model_width*/ && y_distance < verticalDistance/*_model_height*/;
    }

    void World::coinEaten(std::shared_ptr<Coin> coin) {
        _coins.erase(std::remove(_coins.begin(), _coins.end(), coin), _coins.end());
        _score->onNotify(EventType::CoinEaten);

        std::cout << coin.use_count() << std::endl;
    }

    bool World::isWall(Direction direction) const {
        return std::ranges::any_of(_walls, [direction, this](const std::shared_ptr<Wall> &w) {
            PacMan *temp_pac = new PacMan(*_pacman.get());
            bool is_wall;
            switch (direction) {
                case Direction::Down:
                    temp_pac->move(0, _pacman->getSpeed() * Stopwatch::getInstance().dt());
                    is_wall = (temp_pac->getPosition().second - w.get()->getPosition().second <= _model_height) && (
                                  temp_pac->getPosition().first - w.get()->getPosition().first) <= 0.1;
                    break;
            }

            delete temp_pac;
            return is_wall;
        });
    }
}
