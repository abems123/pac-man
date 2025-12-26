//
// Created by abdellah on 12/11/25.
//

#include "../include/world/World.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>

#include <utils/ResourceManager.h>

#include "../../../app/include/utils/Constants.h"

#include "entities/Wall.h"
#include "entities/collectable/Coin.h"
#include "entities/collectable/Fruit.h"
#include "entities/ghost/Ghost.h"
#include "patterns/AbstractFactory.h"
#include "score/Score.h"

namespace logic {

World::World(const std::shared_ptr<AbstractFactory>& factory)
    : _score(std::make_unique<Score>()), _factory(factory), _map(ResourceManager::getMap()),
      _rows(static_cast<int>(_map.size())), _cols(static_cast<int>(_map.front().size())), _model_width(2.0f / _cols),
      _model_height(2.0f / _rows) {
    // =========== Build the world from the map [START] ===========
    parseMap();
    computeHouseRegion();
    setWallsTypes();
    // =========== Build the world from the map [END] ===========
}

void World::updateGhostRelease(float dt) {
    // =========== Release ghosts with simple timers [START] ===========
    _level_time += dt;

    if (!_released_ghost_3 && _level_time >= 5.f && _ghosts.size() > 2) {
        _ghosts[2]->setMode(GhostMode::Chase);
        _released_ghost_3 = true;
    }

    if (!_released_ghost_4 && _level_time >= 10.f && _ghosts.size() > 3) {
        _ghosts[3]->setMode(GhostMode::Chase);
        _released_ghost_4 = true;
    }
    // =========== Release ghosts with simple timers [END] ===========
}

void World::parseMap() {
    // =========== Allocate grids and spawn entities [START] ===========
    wall_at.assign(_rows, std::vector<std::shared_ptr<Wall>>(_cols, nullptr));
    collectable_at.assign(_rows, std::vector<std::shared_ptr<Collectable>>(_cols, nullptr));
    gate_at.assign(_rows, std::vector<bool>(_cols, false));
    house_at.assign(_rows, std::vector<bool>(_cols, false));

    for (int r = 0; r < _rows; ++r) {
        for (int c = 0; c < _cols; ++c) {
            const float x = xFromCol(c);
            const float y = yFromRow(r);

            const char ch = _map[r][c];

            if (ch == '#') {
                auto wall = _factory->createWall(x, y);
                _walls.push_back(wall);
                wall_at[r][c] = wall;
            } else if (ch == '*') {
                auto coin = _factory->createCoin(x, y);
                collectable_at[r][c] = coin;
                _collectables.push_back(coin);
            } else if (ch == '$') {
                auto fruit = _factory->createFruit(x, y);
                collectable_at[r][c] = fruit;
                _collectables.push_back(fruit);
            } else if (ch == '&') {
                gate_at[r][c] = true;
                _gate_cells.emplace_back(r, c);
            } else if (ch == 'P') {
                _pacman = _factory->createPacMan(x, y);
            } else if (ch == '@') {
                ghost_spawns.emplace_back(r, c);
                auto ghost = _factory->createGhost(x, y);
                _ghosts.push_back(ghost);
            }
        }
    }
    // =========== Allocate grids and spawn entities [END] ===========
}

bool World::isGateCell(int r, int c) const {
    if (r < 0 || r >= _rows || c < 0 || c >= _cols)
        return false;
    return gate_at[r][c];
}

bool World::isHouseCell(int r, int c) const {
    if (r < 0 || r >= _rows || c < 0 || c >= _cols)
        return false;
    return house_at[r][c];
}

bool World::isBlockedForPacman(int r, int c) const { return isWallCell(r, c) || isGateCell(r, c) || isHouseCell(r, c); }

void World::changePacmanDirection(Direction d) { _current_dir = d; }

void World::computeHouseRegion() {
    // =========== Flood fill the ghost house interior [START] ===========
    for (int r = 0; r < _rows; ++r)
        std::fill(house_at[r].begin(), house_at[r].end(), false);

    std::queue<std::pair<int, int>> q;

    auto push = [&](int r, int c) {
        if (r < 0 || r >= _rows || c < 0 || c >= _cols)
            return;
        if (house_at[r][c])
            return;
        if (isWallCell(r, c))
            return;
        if (isGateCell(r, c))
            return; // gate is not interior

        house_at[r][c] = true;
        q.emplace(r, c);
    };

    for (auto [sr, sc] : ghost_spawns)
        push(sr, sc);

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        push(r - 1, c);
        push(r + 1, c);
        push(r, c - 1);
        push(r, c + 1);
    }
    // =========== Flood fill the ghost house interior [END] ===========
}

void World::collect() {
    if (!_pacman)
        return;

    // =========== Eat collectables using Pac-Man AABB [START] ===========
    const float px = _pacman->getPosition().first;
    const float py = _pacman->getPosition().second;

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
    // =========== Eat collectables using Pac-Man AABB [END] ===========
}

void World::snapVertically(int x) const { _pacman->setPosition(xFromCol(x), _pacman->getPosition().second); }

void World::snapHorizontally(int y) const { _pacman->setPosition(_pacman->getPosition().first, yFromRow(y)); }

bool World::isWallCell(int r, int c) const {
    return r < 0 || r >= _rows || c < 0 || c >= _cols || wall_at[r][c] != nullptr;
}

std::set<Direction> World::getAvailableDirectionsAt(int row, int col) const {
    std::set<Direction> dirs;

    if (!isBlockedForPacman(row - 1, col))
        dirs.insert(Direction::Up);
    if (!isBlockedForPacman(row + 1, col))
        dirs.insert(Direction::Down);
    if (!isBlockedForPacman(row, col - 1))
        dirs.insert(Direction::Left);
    if (!isBlockedForPacman(row, col + 1))
        dirs.insert(Direction::Right);

    return dirs;
}

void World::handleCollision(const float dt) const {
    if (!_pacman)
        return;

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
    default:
        break;
    }

    int col = colFromX(cx);
    int row = rowFromY(cy);

    const float cellLeft = xFromCol(col);
    const float cellTop = yFromRow(row);

    const float cellCx = cellLeft + halfW;
    const float cellCy = cellTop + halfH;

    // ================ TURN ONLY when aligned enough [START] ================
    if (_current_dir != _pacman->getDirection()) {
        const bool wantHorizontal = isHorizontal(_current_dir);
        const bool wantVertical = isVertical(_current_dir);

        const bool alignedForHorizontal = std::fabs(getCenter(_pacman.get()).second - cellCy) <= turnEpsY;
        const bool alignedForVertical = std::fabs(getCenter(_pacman.get()).first - cellCx) <= turnEpsX;

        auto available = getAvailableDirectionsAt(row, col);
        const bool targetFree = available.contains(_current_dir);

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
    // ================ TURN ONLY when aligned enough [END] ================

    const Direction dir = _pacman->getDirection();

    float realCx = getCenter(_pacman.get()).first;
    float realCy = getCenter(_pacman.get()).second;

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
    default:
        break;
    }

    col = colFromX(scx);
    row = rowFromY(scy);

    const float left = xFromCol(col);
    const float top = yFromRow(row);
    const float right = left + _model_width;
    const float bottom = top + _model_height;

    // ==================================== PREDICTIVE BLOCKING [START] ====================================
    const float step = dt * _pacman->getSpeed();

    _pacman->setMove(Direction::Left, !isBlockedForPacman(row, col - 1) || (realCx - halfW) > left);
    _pacman->setMove(Direction::Right, !isBlockedForPacman(row, col + 1) || (realCx + halfW) < right);
    _pacman->setMove(Direction::Up, !isBlockedForPacman(row - 1, col) || (realCy - halfH) > top);
    _pacman->setMove(Direction::Down, !isBlockedForPacman(row + 1, col) || (realCy + halfH) < bottom);

    if (dir == Direction::Left) {
        const float nextCx = realCx - step;
        if (isBlockedForPacman(row, col - 1) && (nextCx - halfW) < left) {
            _pacman->setMove(Direction::Left, false);
            _pacman->setPosition(left, _pacman->getPosition().second);
        }
    } else if (dir == Direction::Right) {
        const float nextCx = realCx + step;
        if (isBlockedForPacman(row, col + 1) && (nextCx + halfW) > right) {
            _pacman->setMove(Direction::Right, false);
            _pacman->setPosition(right - _model_width, _pacman->getPosition().second);
        }
    } else if (dir == Direction::Up) {
        const float nextCy = realCy - step;
        if (isBlockedForPacman(row - 1, col) && (nextCy - halfH) < top) {
            _pacman->setMove(Direction::Up, false);
            _pacman->setPosition(_pacman->getPosition().first, top);
        }
    } else if (dir == Direction::Down) {
        const float nextCy = realCy + step;
        if (isBlockedForPacman(row + 1, col) && (nextCy + halfH) > bottom) {
            _pacman->setMove(Direction::Down, false);
            _pacman->setPosition(_pacman->getPosition().first, bottom - _model_height);
        }
    }
    // ==================================== PREDICTIVE BLOCKING [END] ====================================
}

int World::colFromX(float x) const { return static_cast<int>((x + 1.0f) / _model_width); }
int World::rowFromY(float y) const { return static_cast<int>((y + 1.0f) / _model_height); }

void World::update(float dt) {
    updateGhostRelease(dt);

    // =========== Tick down global frightened timer [START] ===========
    if (_frightened_left > 0.f) {
        _frightened_left = std::max(0.f, _frightened_left - dt);
    }
    // =========== Tick down global frightened timer [END] ===========

    // =========== Update ghosts first (they read the world timer) [START] ===========
    for (const auto& g : _ghosts)
        g->update(this, dt);
    // =========== Update ghosts first (they read the world timer) [END] ===========

    if (!_pacman)
        return;

    if (_current_dir == Direction::None)
        return;

    handleCollision(dt);
    _pacman->update(dt);
    collect();
}

void World::eat(const std::shared_ptr<Collectable>& collectable) {
    // =========== Remove from grid + list [START] ===========
    const float cx = collectable->getPosition().first + _model_width * 0.5f;
    const float cy = collectable->getPosition().second + _model_height * 0.5f;

    const int col = std::clamp(colFromX(cx), 0, _cols - 1);
    const int row = std::clamp(rowFromY(cy), 0, _rows - 1);

    if (collectable_at[row][col] == collectable) {
        collectable_at[row][col].reset();
    }

    std::erase(_collectables, collectable);
    // =========== Remove from grid + list [END] ===========

    // =========== Score + frightened trigger [START] ===========
    if (collectable->getScore() == Constants::COIN_SCORE) {
        _score->onNotify(EventType::CoinEaten);
    } else if (collectable->getScore() == Constants::FRUIT_SCORE) {
        _score->onNotify(EventType::FruitEaten);
        startFrightened(_frightened_duration);
    }
    // =========== Score + frightened trigger [END] ===========
}

bool World::collides(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

bool World::collides(const EntityModel* m1, const EntityModel* m2) const {
    return collides(m1->getPosition().first, m1->getPosition().second, _model_width, _model_height,
                    m2->getPosition().first, m2->getPosition().second, _model_width, _model_height);
}

std::pair<float, float> World::getCenter(const EntityModel* model) const {
    return {model->getPosition().first + _model_width / 2.0f, model->getPosition().second + _model_height / 2.0f};
}

void World::setWallsTypes() const {
    // =========== Pick wall types based on neighbors [START] ===========
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
    // =========== Pick wall types based on neighbors [END] ===========
}

float World::getFrightenedLeft() const { return _frightened_left; }
const std::vector<std::shared_ptr<Ghost>>& World::getGhosts() const { return _ghosts; }
const std::vector<std::pair<int, int>>& World::getGateCells() const { return _gate_cells; }
const PacMan* World::getPacMan() const { return _pacman.get(); }
Score* World::getScore() const { return _score.get(); }
int World::getRows() const { return _rows; }
int World::getCols() const { return _cols; }

void World::startFrightened(float duration) {
    // =========== Extend global frightened timer [START] ===========
    _frightened_left = std::max(_frightened_left, duration);
    // =========== Extend global frightened timer [END] ===========

    // =========== Make every ghost react instantly [START] ===========
    for (auto& g : _ghosts) {
        if (g)
            g->enterFrightened(duration);
    }
    // =========== Make every ghost react instantly [END] ===========
}

std::set<Direction> World::getAvailableGhostDirectionsAt(int row, int col, const Ghost* ghost) const {
    std::set<Direction> dirs;
    if (!ghost)
        return dirs;

    auto can_step = [&](int fr, int fc, int tr, int tc) {
        if (isWallCell(tr, tc))
            return false;

        const bool from_house_side = isHouseCell(fr, fc) || isGateCell(fr, fc);
        const bool to_house_side = isHouseCell(tr, tc) || isGateCell(tr, tc);

        // outside -> (house/gate) : never
        if (!from_house_side && to_house_side)
            return false;

        // (house/gate) -> outside : only if allowed AND only from the gate tile itself
        if (from_house_side && !to_house_side) {
            if (ghost->getMode() == GhostMode::Center)
                return false;
            if (!isGateCell(fr, fc))
                return false;
        }

        // gate -> house : don't go back in once released
        if (isGateCell(fr, fc) && isHouseCell(tr, tc) && ghost->getMode() != GhostMode::Center)
            return false;

        return true;
    };

    if (can_step(row, col, row - 1, col))
        dirs.insert(Direction::Up);
    if (can_step(row, col, row + 1, col))
        dirs.insert(Direction::Down);
    if (can_step(row, col, row, col - 1))
        dirs.insert(Direction::Left);
    if (can_step(row, col, row, col + 1))
        dirs.insert(Direction::Right);

    return dirs;
}

float World::xFromCol(int c) const { return -1.f + c * _model_width; }
float World::yFromRow(int r) const { return -1.f + r * _model_height; }

} // namespace logic
