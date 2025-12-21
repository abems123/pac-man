//
// Created by abdellah on 12/11/25.
//

#include "../include/world/World.h"

// #include "../../../app/include/utils/ResourceManager.h"

#include <iostream>

#include "entities/PacMan.h"
#include "entities/Wall.h"
#include "entities/collectable/Coin.h"
#include "entities/collectable/Fruit.h"
#include "patterns/AbstractFactory.h"
#include "score/Score.h"
#include <algorithm>
#include <utils/ResourceManager.h>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "../../../app/include/utils/Constants.h"
#include "entities/ghost/Ghost.h"
#include "entities/ghost/LockedGhost.h"

namespace logic {
    void World::parseMap() {
        // =========== Parsing assets/map.txt to generate the world [START] ===========
        for (int i = 0, n = map.size(); i < n; ++i) {
            wall_at.emplace_back();
            wall_at.back().resize(m, nullptr);

            for (int j = 0, m = map[i].length(); j < m; ++j) {
                const float current_x = 2.0 * j / m - 1; // (static_cast<float>(j) - m / 2.0) / (m / 2.0);
                const float current_y = 2.0 * i / n - 1; // (static_cast<float>(i) - n / 2.0) / (n / 2.0);

                auto current_c = map[i][j];

                if (current_c == '#') {
                    auto wall = _factory->createWall(current_x, current_y);
                    _walls.push_back(wall);
                    wall_at[i][j] = wall;
                } else if (current_c == '*') {
                    auto coin = _factory->createCoin(current_x, current_y);
                    _collectables.push_back(coin);
                } /*else if (current_c == '&') {
                  auto barrier = _factory->createBarrier(current_x, current_y);
                  _barriers.push_back(barrier);
              }*/
                else if (current_c == 'P') {
                    _pacman = _factory->createPacMan(current_x, current_y);
                } else if (current_c == '$') {
                    auto fruit = _factory->createFruit(current_x, current_y);
                    _collectables.push_back(fruit);
                } else if (current_c == '@') {
                    auto ghost = _factory->createGhost(current_x, current_y);
                    if (ghost->getType() == EntityType::LockedGhost) {
                        _locked_ghost = ghost;
                    } else if (ghost->getType() == EntityType::PredictiveGhost) {
                        _predictive_ghost.push_back(ghost);
                    } else if (ghost->getType() == EntityType::FollowerGhost) {
                        _direct_ghost = ghost;
                    }
                } else if (current_c == 'E') {
                }
            }
        }
        // =========== Parsing assets/map.txt to generate the world [END] ===========
    }

    void World::startGhosts() {
    }

    bool World::isIntersection(int r, int c, Direction currentDir) const {
        auto dirs = getViableDirections(r, c);

        // remove backward direction
        auto opposite = oppositeOf(currentDir);
        std::erase(dirs, opposite);

        return dirs.size() >= 2;
    }

    Direction World::oppositeOf(Direction d) const {
        switch (d) {
            case Direction::Up:    return Direction::Down;
            case Direction::Down:  return Direction::Up;
            case Direction::Left:  return Direction::Right;
            case Direction::Right: return Direction::Left;
        }
        return Direction::Up; // unreachable
    }

    void World::updateGhost(Ghost& g) {
        const int r = rowFromY(g.getPosition().second);
        const int c = colFromX(g.getPosition().first);

        auto viable = getViableDirections(r, c);

        bool blocked = !std::ranges::any_of(viable, [&](Direction d) {
            return d == g.getDirection();
        });

        if (blocked || isIntersection(r, c, g.getDirection())) {
            g.clearDirections();
            for (auto d : viable)
                g.addAvailableDir(d);

            g.decideDirection();
        }

        g.move();
    }


    World::World(const std::shared_ptr<AbstractFactory> &factory)
        : _score(std::make_unique<Score>()), _factory(factory), map(ResourceManager::getMap()),
          n(static_cast<int>(map.size())),
          m(static_cast<int>(map.front().size())),
          _model_width(2.0f / m), _model_height(2.0f / n) {
        parseMap();
        _pacman->setSpeed(std::min(_model_width, _model_height) * 4.f);
        setWallsTypes();

        startGhosts();
    }

    void World::movePacMan(Direction direction) {
        const float dt = Stopwatch::getInstance().dt();
        const float dist = _pacman->getSpeed() * dt;

        float dx = (direction == Direction::Left ? -1.f : direction == Direction::Right ? 1.f : 0.f) * dist;

        float dy = (direction == Direction::Up ? -1.f : direction == Direction::Down ? 1.f : 0.f) * dist;


        float nextX = _pacman->getPosition().first + dx;
        float nextY = _pacman->getPosition().second + dy;

        // TODO: make the sprites of pacman 0.05 smaller to fit well
        constexpr float PAC_HITBOX_SCALE = 0.95f; // 95% of tile
        const float pacW = _model_width * PAC_HITBOX_SCALE;
        const float pacH = _model_height * PAC_HITBOX_SCALE;

        // BLOCK movement BEFORE it happens
        if (std::ranges::any_of(_walls, [=](const std::shared_ptr<Wall> &w) {
            const float wallX = w->getPosition().first;
            const float wallY = w->getPosition().second;


            const float pacX = nextX + (_model_width - pacW) / 2.f;
            const float pacY = nextY + (_model_height - pacH) / 2.f;

            return pacX < wallX + _model_width &&
                   pacX + pacW > wallX &&
                   pacY < wallY + _model_height &&
                   pacY + pacH > wallY;
        })) {
            return;
        }


        // Safe to move
        _pacman->setPosition(nextX, nextY);
        _pacman->setDirection(direction);


        collect();
    }

    void World::collect() {
        const auto _close_collectable = std::ranges::find_if(_collectables,
                                                             [this](const std::shared_ptr<Collectable> &c) {
                                                                 return intersects(
                                                                     _pacman.get()->getPosition().first,
                                                                     _pacman.get()->getPosition().second, _model_width,
                                                                     _model_height
                                                                     , c->getPosition().first, c->getPosition().second,
                                                                     _model_width,
                                                                     _model_height);
                                                             });
        if (_close_collectable != _collectables.end()) {
            eat(*_close_collectable);
        }
    }

    std::vector<Direction> World::getViableDirections(
        int c, int r) const
    {
        std::vector<Direction> result;

        auto inside = [&](int r, int c) {
            return r >= 0 && r < n && c >= 0 && c < m;
        };

        auto isFree = [&](int r, int c) {
            if (!inside(r, c)) return false;
            // nullptr == empty tile => free
            return !wall_at[r][c];
        };


        if (isFree(r - 1, c)) result.push_back(Direction::Up);
        if (isFree(r + 1, c)) result.push_back(Direction::Down);
        if (isFree(r, c - 1)) result.push_back(Direction::Left);
        if (isFree(r, c + 1)) result.push_back(Direction::Right);

        return result;
    }

    std::vector<Direction> World::getViableDirections(
        const std::shared_ptr<MovableEntityModel>& entityModel) const
    {
        std::vector<Direction> result;

        auto inside = [&](int r, int c) {
            return r >= 0 && r < n && c >= 0 && c < m;
        };

        auto isFree = [&](int r, int c) {
            if (!inside(r, c)) return false;
            // nullptr == empty tile => free
            return !wall_at[r][c];
        };

        const int c = colFromX(entityModel->getPosition().first);
        const int r = rowFromY(entityModel->getPosition().second);

        if (isFree(r - 1, c)) result.push_back(Direction::Up);
        if (isFree(r + 1, c)) result.push_back(Direction::Down);
        if (isFree(r, c - 1)) result.push_back(Direction::Left);
        if (isFree(r, c + 1)) result.push_back(Direction::Right);

        return result;
    }

    void World::update() {
        const float dt = Stopwatch::getInstance().dt();
        const float speed = _locked_ghost->getSpeed();
        const float dist = speed * dt;

        float dx = 0.f, dy = 0.f;
        switch (_locked_ghost->getDirection()) {
            case Direction::Up: dy = -dist;
                break;
            case Direction::Down: dy = dist;
                break;
            case Direction::Left: dx = -dist;
                break;
            case Direction::Right: dx = dist;
                break;
        }

        const float nextX = _locked_ghost->getPosition().first + dx;
        const float nextY = _locked_ghost->getPosition().second + dy;

        bool collided = std::any_of(_walls.begin(), _walls.end(),
                                    [&](const std::shared_ptr<Wall> &w) {
                                        return intersects(
                                            nextX, nextY, _model_width, _model_height,
                                            w->getPosition().first, w->getPosition().second,
                                            _model_width, _model_height
                                        );
                                    });


        if (collided) {
            const sf::Vector2i coords = {
                colFromX(_locked_ghost->getPosition().first),
                rowFromY(_locked_ghost->getPosition().second)
            };

            auto viable = getViableDirections(_locked_ghost);
            _locked_ghost->clearDirections();

            for (auto d: viable)
                _locked_ghost->addAvailableDir(d);

            _locked_ghost->decideDirection();
            return; // do NOT move this frame
        }

        _locked_ghost->setPosition(nextX, nextY);
    }


    bool World::intersects(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const {
        return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
    }

    void World::eat(const std::shared_ptr<Collectable> &collectable) {
        std::erase(_collectables, collectable);

        if (collectable->getScore() == Constants::COIN_SCORE)
            _score->onNotify(EventType::CoinEaten);
        else if (collectable->getScore() == Constants::FRUIT_SCORE)
            _score->onNotify(EventType::FruitEaten);
    }


    int World::colFromX(float x) const {
        int col = static_cast<int>(std::floor((x + 1.f) / _model_width));
        return std::clamp(col, 0, m - 1);
    }

    int World::rowFromY(float y) const {
        // y: -1 (top) -> 0,  +1 (bottom) -> n-1
        int row = static_cast<int>(std::floor((y + 1.f) / _model_height));
        return std::clamp(row, 0, n - 1);
    }

    float World::tileLeftX(int col) const {
        return -1.f + col * _model_width;
    }

    bool World::isWallCell(int r, int c) const {
        return r >= 0 && r < n && c >= 0 && c < m && wall_at[r][c];
    }

    bool World::collides(const EntityModel *m1, const EntityModel *m2) const {
        return intersects(m1->getPosition().first, m1->getPosition().second, _model_width, _model_height,
                          m2->getPosition().first, m2->getPosition().second, _model_width, _model_height);
    }

    void World::setWallsTypes() const {
        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < m; ++j) {
                if (!isWallCell(i, j))
                    continue;

                auto *current_wall = wall_at[i][j].get();

                if (!current_wall) {
                    std::cerr << "Wall is null" << std::endl;
                    return;
                }
                const bool N = isWallCell(i - 1, j);
                const bool E = isWallCell(i, j + 1);
                const bool S = isWallCell(i + 1, j);
                const bool W = isWallCell(i, j - 1);

                const int mask = (N << 3) | (E << 2) | (S << 1) | (W << 0);

                WallType type = WallType::Isolated;

                switch (mask) {
                    case 1:
                        type = WallType::EndLeft;
                        break;
                    case 2:
                        type = WallType::EndDown;
                        break;
                    case 4:
                        type = WallType::EndRight;
                        break;
                    case 8:
                        type = WallType::EndUp;
                        break;

                    case 5:
                        type = WallType::Horizontal;
                        break;
                    case 10:
                        type = WallType::Vertical;
                        break;

                    case 3:
                        type = WallType::CornerSW;
                        break;
                    case 6:
                        type = WallType::CornerSE;
                        break;
                    case 9:
                        type = WallType::CornerNW;
                        break;
                    case 12:
                        type = WallType::CornerNE;
                        break;
                    case 7:
                        type = WallType::T_S;
                        break;
                    case 11:
                        type = WallType::T_W;
                        break;
                    case 13:
                        type = WallType::T_N;
                        break;
                    case 14:
                        type = WallType::T_E;
                        break;

                    case 15:
                        type = WallType::Cross;
                        break;
                    default:
                        type = WallType::Isolated;
                        break;
                }

                current_wall->setWallType(type);
            }
        }
    }
} // namespace logic
