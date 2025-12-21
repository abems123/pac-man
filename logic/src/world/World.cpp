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

#include "../../../app/include/utils/Constants.h"
#include "entities/ghost/Ghost.h"
#include "entities/ghost/LockedGhost.h"
#include "utils/Constants.h"

namespace logic {
    void World::parseMap() {
        // =========== Parsing assets/map.txt to generate the world [START] ===========
        for (int i = 0, n = map.size(); i < n; ++i) {
            for (int j = 0, m = map[i].length(); j < m; ++j) {
                const float current_x = 2.0 * j / m - 1; // (static_cast<float>(j) - m / 2.0) / (m / 2.0);
                const float current_y = 2.0 * i / n - 1; // (static_cast<float>(i) - n / 2.0) / (n / 2.0);

                auto current_c = map[i][j];

                if (current_c == '#') {
                    auto wall = _factory->createWall(current_x, current_y);
                    _walls.push_back(wall);
                    wall_at[i][j] = wall; // store
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
                    if (ghost->getType() == GhostType::Locked) {
                        _locked_ghost = ghost;
                    } else if (ghost->getType() == GhostType::Predictive) {
                        _predictive_ghost.push_back(ghost);
                    } else if (ghost->getType() == GhostType::Follower) {
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

    World::World(const std::shared_ptr<AbstractFactory> &factory)
        : _score(std::make_unique<Score>()), _factory(factory), map(ResourceManager::getMap()),
          n(static_cast<int>(map.size())),
          m(static_cast<int>(map.front().size())),
          _model_width(2.0f / m), _model_height(2.0f / n),
          wall_at(std::vector(n, std::vector<std::shared_ptr<Wall> >(m))) {
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


        // TODO: do the logic for snapping pacman when in an edge
        if (direction == Direction::Up || direction == Direction::Down) {
            // Use Pac-Man's current center X to pick the intended column
            const float curCenterX = _pacman->getPosition().first + _model_width / 2.f;
            const int col = colFromX(curCenterX);

            // If 80% aligned with this column, snap X to the tile's left edge (your coordinate convention)
            if (shouldSnapToColumn(nextX, col, pacW)) {
                nextX = tileLeftX(col);
            }

            // Optional: also allow snapping to neighboring column if it's closer
            // (helps when you're between columns)
            const int colL = std::max(col - 1, 0);
            const int colR = std::min(col + 1, m - 1);

            if (shouldSnapToColumn(nextX, colL, pacW)) nextX = tileLeftX(colL);
            if (shouldSnapToColumn(nextX, colR, pacW)) nextX = tileLeftX(colR);
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

    void World::update() {
        _locked_ghost->update();

        bool collided = std::any_of(_walls.begin(), _walls.end(), [this](const std::shared_ptr<Wall> &w) {
            if (collides(_locked_ghost.get(), w.get())) {
                std::cout << "collided" << std::endl;
            }
            return collides(_locked_ghost.get(), w.get());
        });

        if (collided) {
            // _locked_ghost->getDirectionRandomly({});
        }
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


    static constexpr float SNAP_THRESHOLD = 0.5; // 80%

    bool World::canSnapVertically(float pacX, float pacY, float &snappedX) const {
        for (const auto &wL: _walls) {
            for (const auto &wR: _walls) {
                // walls on same row
                if (std::abs(wL->getPosition().second - wR->getPosition().second) > 0.01f)
                    continue;

                // left and right walls around pacman
                if (wL->getPosition().first >= wR->getPosition().first)
                    continue;

                float gapLeft = wL->getPosition().first + _model_width;
                float gapRight = wR->getPosition().first;

                float gapWidth = gapRight - gapLeft;

                // corridor wide enough
                if (gapWidth < _model_width)
                    continue;

                float corridorCenter = (gapLeft + gapRight) / 2.f;

                float pacCenter = pacX + _model_width / 2.f;
                float offset = std::abs(pacCenter - corridorCenter);

                if (offset <= (gapWidth - _model_width) * SNAP_THRESHOLD) {
                    snappedX = corridorCenter - _model_width / 2.f;
                    return true;
                }
            }
        }
        return false;
    }

    int World::colFromX(float x) const {
        // x is in [-1, 1], tile width = _model_width
        int col = static_cast<int>(std::floor((x + 1.f) / _model_width));
        return std::clamp(col, 0, m - 1);
    }

    float World::tileLeftX(int col) const {
        return -1.f + col * _model_width; // matches your current_x formula
    }

    bool World::shouldSnapToColumn(float nextX, int col, float pacW) const {
        const float tileL = tileLeftX(col);
        const float tileR = tileL + _model_width;

        const float pacL = nextX + (_model_width - pacW) / 2.f;
        const float pacR = pacL + pacW;

        const float overlap = std::min(pacR, tileR) - std::max(pacL, tileL);
        return overlap >= 0.8f * pacW;
    }

    bool World::isWallCell(int r, int c) const {
        return r >= 0 && r < n && c >= 0 && c < m && wall_at[r][c] != nullptr;
    }

    bool World::collides(const EntityModel *m1, const EntityModel *m2) const {
        return intersects(m1->getPosition().first, m1->getPosition().second, _model_width, _model_height,
                          m2->getPosition().first, m2->getPosition().second, _model_width, _model_height);
    }

    void World::setWallsTypes() const {
        auto isWall = [&](int r, int c) { return r >= 0 && r < n && c >= 0 && c < m && wall_at[r][c] != nullptr; };

        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < m; ++j) {
                if (!wall_at[i][j])
                    continue;


                const bool N = isWall(i - 1, j);
                const bool E = isWall(i, j + 1);
                const bool S = isWall(i + 1, j);
                const bool W = isWall(i, j - 1);

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

                wall_at[i][j]->setType(type);
            }
        }
    }
} // namespace logic
