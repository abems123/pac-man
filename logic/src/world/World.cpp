//
// Created by abdellah on 12/11/25.
//

#include "../include/world/World.h"

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

#include <queue>

namespace logic {

World::World(const std::shared_ptr<AbstractFactory>& factory)
    : _score(std::make_unique<Score>()), _factory(factory), _map(ResourceManager::getMap()),
      _rows(static_cast<int>(_map.size())), _cols(static_cast<int>(_map.front().size())), _model_width(2.0f / _cols),
      _model_height(2.0f / _rows) {

    parseMap();
    setWallsTypes();
}

void World::parseMap() {
    // =========== Parsing assets/map.txt to generate the world [START] ===========
    for (int i = 0; i < _rows; ++i) {
        wall_at.emplace_back(); wall_at.back().resize(_cols, nullptr);
        collectable_at.emplace_back(); collectable_at.back().resize(_cols, nullptr);


        for (int j = 0; j < _cols; ++j) {
            const float current_x = xFromCol(j);
            const float current_y = yFromRow(i);

            auto current_c = _map[i][j];

            if (current_c == '#') {
                auto wall = _factory->createWall(current_x, current_y);
                _walls.push_back(wall);
                wall_at[i][j] = wall;
            } else if (current_c == '*') {
                auto coin = _factory->createCoin(current_x, current_y);
                collectable_at[i][j] = coin;
                _collectables.push_back(coin);
            } else if (current_c == '&') {
                // auto barrier = _factory->createBarrier(current_x, current_y);
                // _barriers.push_back(barrier);
            } else if (current_c == 'P') {
                _pacman = _factory->createPacMan(current_x, current_y);
            } else if (current_c == '$') {
                auto fruit = _factory->createFruit(current_x, current_y);
                collectable_at[i][j] = fruit;
                _collectables.push_back(fruit);
            } else if (current_c == '@') {
                auto ghost = _factory->createGhost(current_x, current_y);
                _ghosts.push_back(ghost);
            } else if (current_c == 'E') {
                // Empty space - do nothing
            }
        }
    }
    // =========== Parsing assets/map.txt to generate the world [END] ===========
}

void World::changePacmanDirection(Direction d) { _current_dir = d; }

void World::collect() {
    if (!_pacman)
        return;

    // Pac-Man AABB (top-left + tile size)
    const float px = _pacman->getPosition().first;
    const float py = _pacman->getPosition().second;

    // Avoid exact-border indexing issues
    const float tiny = 1e-4f;

    const int c0 = std::clamp(colFromX(px + tiny), 0, _cols - 1);
    const int c1 = std::clamp(colFromX(px + _model_width - tiny), 0, _cols - 1);
    const int r0 = std::clamp(rowFromY(py + tiny), 0, _rows - 1);
    const int r1 = std::clamp(rowFromY(py + _model_height - tiny), 0, _rows - 1);

    for (int r = r0; r <= r1; ++r) {
        for (int c = c0; c <= c1; ++c) {
            auto& cell = collectable_at[r][c];
            if (cell && collides(_pacman.get(), cell.get())) {
                auto eaten = cell;
                cell.reset();
                eat(eaten);
                return;
            }
        }
    }
}

void World::snapVertically(int x) const { _pacman->setPosition(xFromCol(x), _pacman->getPosition().second); }

void World::snapHorizontally(int y) const { _pacman->setPosition(_pacman->getPosition().first, yFromRow(y)); }

bool World::isWallCell(int r, int c) const {
    return r < 0 || r >= _rows || c < 0 || c >= _cols || wall_at[r][c] != nullptr;
}

std::set<Direction> World::getAvailableDirectionsAt(int row, int col) const {
    std::set<Direction> dirs;
    if (!isWallCell(row - 1, col))
        dirs.insert(Direction::Up);
    if (!isWallCell(row + 1, col))
        dirs.insert(Direction::Down);
    if (!isWallCell(row, col - 1))
        dirs.insert(Direction::Left);
    if (!isWallCell(row, col + 1))
        dirs.insert(Direction::Right);
    return dirs;
}

void World::handleCollision(const float dt) const {
    auto isHorizontal = [](Direction d) { return d == Direction::Left || d == Direction::Right; };
    auto isVertical = [](Direction d) { return d == Direction::Up || d == Direction::Down; };

    const float halfW = _model_width * 0.5f;
    const float halfH = _model_height * 0.5f;

    const float turnEpsX = std::max(_model_width * 0.05f, dt * _pacman->getSpeed());
    const float turnEpsY = std::max(_model_height * 0.05f, dt * _pacman->getSpeed());

    // Current cell from center (with tiny bias to avoid edge rounding glitches)
    float cx = getCenter(_pacman.get()).first;
    float cy = getCenter(_pacman.get()).second;

    const float tiny = 1e-4f;
    switch (_pacman->getDirection()) {
    case Direction::Left:
        cx -= tiny;
        break;
    case Direction::Right:
        cx += tiny;
        break;
    case Direction::Up:
        cy -= tiny;
        break;
    case Direction::Down:
        cy += tiny;
        break;
    }

    int col = colFromX(cx);
    int row = rowFromY(cy);

    const float cellLeft = xFromCol(col);
    const float cellTop = yFromRow(row);

    const float cellCx = cellLeft + halfW;
    const float cellCy = cellTop + halfH;

    // ================ TURN ONLY when aligned enough [START] ================
    // current direction continues if turn isn't allowed yet
    if (_current_dir != _pacman->getDirection()) {
        const bool wantHorizontal = isHorizontal(_current_dir);
        const bool wantVertical = isVertical(_current_dir);

        const bool alignedForHorizontal = std::fabs(getCenter(_pacman.get()).second - cellCy) <= turnEpsY;
        const bool alignedForVertical = std::fabs(getCenter(_pacman.get()).first - cellCx) <= turnEpsX;

        // check target cell is free
        auto available = getAvailableDirectionsAt(row, col);
        bool targetFree = available.contains(_current_dir);

        // =============== Snap + apply the turn [START] ===============
        if (targetFree) {
            if (wantHorizontal && alignedForHorizontal) {
                snapHorizontally(row);
                _pacman->setDirection(_current_dir);
            } else if (wantVertical && alignedForVertical) {
                snapVertically(col);
                _pacman->setDirection(_current_dir);
            }
        }
        // =============== Snap + apply the turn [END] ===============
    }

    // Recompute after potential snap/turn
    const Direction dir = _pacman->getDirection();

    float realCx = getCenter(_pacman.get()).first;
    float realCy = getCenter(_pacman.get()).second;

    // Recompute row/col from real center (again with direction bias)
    float scx = realCx, scy = realCy;
    switch (dir) {
    case Direction::Left:
        scx -= tiny;
        break;
    case Direction::Right:
        scx += tiny;
        break;
    case Direction::Up:
        scy -= tiny;
        break;
    case Direction::Down:
        scy += tiny;
        break;
    }

    col = colFromX(scx);
    row = rowFromY(scy);

    const float left = xFromCol(col);
    const float top = yFromRow(row);
    const float right = left + _model_width;
    const float bottom = top + _model_height;

    // ==================================== PREDICTIVE BLOCKING [START] ====================================
    // ================= (don’t stop early, only block if next step crosses wall boundary) =================
    const float step = dt * _pacman->getSpeed();

    // Allow moves by default
    _pacman->setMove(Direction::Left, !isWallCell(row, col - 1) || (realCx - halfW) > left);
    _pacman->setMove(Direction::Right, !isWallCell(row, col + 1) || (realCx + halfW) < right);
    _pacman->setMove(Direction::Up, !isWallCell(row - 1, col) || (realCy - halfH) > top);
    _pacman->setMove(Direction::Down, !isWallCell(row + 1, col) || (realCy + halfH) < bottom);

    // Enforce only for the current direction using the *next* position
    if (dir == Direction::Left) {
        const float nextCx = realCx - step;
        if (isWallCell(row, col - 1) && (nextCx - halfW) < left) {
            _pacman->setMove(Direction::Left, false);
            _pacman->setPosition(left, _pacman->getPosition().second); // clamp top-left X
        }
    } else if (dir == Direction::Right) {
        const float nextCx = realCx + step;
        if (isWallCell(row, col + 1) && (nextCx + halfW) > right) {
            _pacman->setMove(Direction::Right, false);
            _pacman->setPosition(right - _model_width, _pacman->getPosition().second);
        }
    } else if (dir == Direction::Up) {
        const float nextCy = realCy - step;
        if (isWallCell(row - 1, col) && (nextCy - halfH) < top) {
            _pacman->setMove(Direction::Up, false);
            _pacman->setPosition(_pacman->getPosition().first, top);
        }
    } else if (dir == Direction::Down) {
        const float nextCy = realCy + step;
        if (isWallCell(row + 1, col) && (nextCy + halfH) > bottom) {
            _pacman->setMove(Direction::Down, false);
            _pacman->setPosition(_pacman->getPosition().first, bottom - _model_height);
        }
    }
    // ==================================== PREDICTIVE BLOCKING [END] ====================================
    // ================= (don’t stop early, only block if next step crosses wall boundary) =================
}

int World::colFromX(float x) const { return static_cast<int>((x + 1.0f) / _model_width); }

int World::rowFromY(float y) const { return static_cast<int>((y + 1.0f) / _model_height); }

void World::update(float dt) {

    // Update ghosts
    for (const auto& g : _ghosts)
        g->update(this, dt);


    if (_current_dir == Direction::None) return;

    handleCollision(dt);

    _pacman->update(dt);

    // Check for collectables
    collect();
}

void World::eat(const std::shared_ptr<Collectable>& collectable) {
    // Clear grid entry as well
    const float cx = collectable->getPosition().first + _model_width * 0.5f;
    const float cy = collectable->getPosition().second + _model_height * 0.5f;
    const int col = std::clamp(colFromX(cx), 0, _cols - 1);
    const int row = std::clamp(rowFromY(cy), 0, _rows - 1);

    if (collectable_at[row][col] == collectable) {
        collectable_at[row][col].reset();
    }

    std::erase(_collectables, collectable);

    if (collectable->getScore() == Constants::COIN_SCORE)
        _score->onNotify(EventType::CoinEaten);
    else if (collectable->getScore() == Constants::FRUIT_SCORE)
        _score->onNotify(EventType::FruitEaten);
}

bool World::collides(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}
bool World::collides(const EntityModel* m1, const EntityModel* m2) const {
    return collides(m1->getPosition().first, m1->getPosition().second, _model_width, _model_height,
                    m2->getPosition().first, m2->getPosition().second, _model_width, _model_height);
}
std::pair<float, float> World::getCenter(const EntityModel* model) const {
    return {model->getPosition().first + _model_width / 2.0, model->getPosition().second + _model_height / 2.0};
}

void World::setWallsTypes() const {
    auto hasWall = [this](int r, int c) {
        return r >= 0 && r < _rows && c >= 0 && c < _cols && wall_at[r][c] != nullptr;
    };

    for (int i = 0; i < _rows; ++i) {
        for (int j = 0; j < _cols; ++j) {
            if (!hasWall(i, j))
                continue;

            auto* current_wall = wall_at[i][j].get();
            if (!current_wall)
                continue;

            const bool N = hasWall(i - 1, j);
            const bool E = hasWall(i, j + 1);
            const bool S = hasWall(i + 1, j);
            const bool W = hasWall(i, j - 1);

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

std::set<Direction> World::getAvailableGhostDirectionsAt(int row, int col, const Ghost* ghost) const {
}

float World::xFromCol(int c) const { return -1.f + c * _model_width; }
float World::yFromRow(int r) const { return -1.f + r * _model_height; }

} // namespace logic
