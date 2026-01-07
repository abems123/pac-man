// Created by abdellah on 12/11/2025.

#include "../include/world/World.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <queue>

#include "entities/Wall.h"
#include "entities/collectable/Coin.h"
#include "entities/collectable/Fruit.h"
#include "entities/ghost/Ghost.h"
#include "patterns/AbstractFactory.h"
#include <events/EventType.h>
#include <utils/FileManager.h>

namespace logic {

namespace {
inline bool inBounds(const int r, const int c, const int rows, const int cols) {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

inline void biasByDir(const Direction d, float& x, float& y, const float tiny) {
    switch (d) {
    case Direction::Left:
        x -= tiny;
        break;
    case Direction::Right:
        x += tiny;
        break;
    case Direction::Up:
        y -= tiny;
        break;
    case Direction::Down:
        y += tiny;
        break;
    default:
        break;
    }
}

inline bool isHorizontal(const Direction d) { return d == Direction::Left || d == Direction::Right; }
inline bool isVertical(const Direction d) { return d == Direction::Up || d == Direction::Down; }
} // namespace

World::World(const std::shared_ptr<AbstractFactory>& factory, const int level, int carryScore, const int carryLives)
    : _lives_left(std::max(1, carryLives)), _score(std::make_unique<Score>("scoreboard.txt", carryScore)),
      _factory(factory), _map(FileReader::getFileLines("../../assets/maps/map.txt")),
      _rows(static_cast<int>(_map.size())), _cols(static_cast<int>(_map.front().size())), _model_width(2.0f / _cols),
      _model_height(2.0f / _rows), _level(std::max(1, level)) {
    parseMap();
    setWallsTypes();
    applyGhostDifficulty();
    applyFrightenedDifficulty();
}

void World::applyFrightenedDifficulty() {
    constexpr float p = std::clamp(Constants::FRIGHTENED_TIME_REDUCTION_PERCENTAGE, 0.f, 0.85f);
    _frightened_duration = Constants::FRIGHTENED_DURATION * std::pow(1.f - p, static_cast<float>(_level - 1));
}

void World::applyGhostDifficulty() {
    const float mult = std::pow(1.f + Constants::GHOST_SPEED_BOOST_PERCENTAGE, static_cast<float>(_level));
    constexpr float ghostMax = Constants::PACMAN_SPEED * 1.1f;
    const float base = std::min(ghostMax, Constants::GHOST_START_SPEED * mult);

    for (auto& g : _ghosts)
        if (g)
            g->setBaseSpeed(base);
}

void World::updateGhostRelease(const float dt) {
    _level_time += dt;

    auto release = [&](const size_t idx, const float t, bool& flag) {
        if (flag || _level_time < t || _ghosts.size() <= idx)
            return;
        if (_ghosts[idx])
            _ghosts[idx]->setMode(GhostMode::Chase);
        flag = true;
    };

    release(2, 5.f, _released_ghost_3);
    release(3, 10.f, _released_ghost_4);
}

void World::parseMap() {
    wall_at.assign(_rows, std::vector<std::shared_ptr<Wall>>(_cols));
    collectable_at.assign(_rows, std::vector<std::shared_ptr<Collectable>>(_cols));
    gate_at.assign(_rows, std::vector(_cols, false));
    house_at.assign(_rows, std::vector(_cols, false));

    for (int r = 0; r < _rows; ++r) {
        for (int c = 0; c < _cols; ++c) {
            const float x = xFromCol(c);
            const float y = yFromRow(r);

            switch (_map[r][c]) {
            case '#': {
                auto w = _factory->createWall(x, y, false);
                _walls.push_back(w);
                wall_at[r][c] = w;
            } break;

            case '*': {
                auto coin = _factory->createCoin(x, y);
                collectable_at[r][c] = coin;
                _collectables.push_back(coin);
                ++_coins_left;
            } break;

            case '$': {
                auto fruit = _factory->createFruit(x, y);
                collectable_at[r][c] = fruit;
                _collectables.push_back(fruit);
                ++_fruits_left;
            } break;

            case '&': {
                auto gate = _factory->createWall(x, y, true);
                _walls.push_back(gate);
                gate_at[r][c] = true;
                _gate_cells.emplace_back(r, c);
            } break;

            case 'P':
                _pacman = _factory->createPacMan(x, y);
                _pacman->setDirection(Direction::None);
                _pac_spawn_x = x;
                _pac_spawn_y = y;
                break;

            case '@':
                _ghosts.push_back(_factory->createGhost(x, y));
                house_at[r][c] = true;
                break;
            default:
                break;
            }
        }
    }
}

bool World::isGateCell(const int r, const int c) const { return inBounds(r, c, _rows, _cols) && gate_at[r][c]; }

bool World::isHouseCell(const int r, const int c) const { return inBounds(r, c, _rows, _cols) && house_at[r][c]; }

bool World::isWallCell(const int r, const int c) const {
    return !inBounds(r, c, _rows, _cols) || wall_at[r][c] != nullptr;
}

bool World::isBlockedForPacman(const int r, const int c) const {
    return isWallCell(r, c) || isGateCell(r, c) || isHouseCell(r, c);
}

void World::changePacmanDirection(const Direction direction) { _current_dir = direction; }

void World::collect() {
    if (!_pacman)
        return;

    const float px = _pacman->getPosition().first;
    const float py = _pacman->getPosition().second;

    constexpr float tiny = 1e-4f;

    const int c0 = std::clamp(colFromX(px + tiny), 0, _cols - 1);
    const int c1 = std::clamp(colFromX(px + _model_width - tiny), 0, _cols - 1);
    const int r0 = std::clamp(rowFromY(py + tiny), 0, _rows - 1);
    const int r1 = std::clamp(rowFromY(py + _model_height - tiny), 0, _rows - 1);

    for (int r = r0; r <= r1; ++r) {
        for (int c = c0; c <= c1; ++c) {
            auto& cell = collectable_at[r][c];
            if (cell && collides(*_pacman, *cell)) {
                const auto eaten = cell;
                cell.reset();
                eat(eaten);
                return;
            }
        }
    }
}

void World::snapVertically(const int x) const { _pacman->setPosition(xFromCol(x), _pacman->getPosition().second); }
void World::snapHorizontally(const int y) const { _pacman->setPosition(_pacman->getPosition().first, yFromRow(y)); }

std::set<Direction> World::getAvailableDirectionsAt(const int row, const int col) const {
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

    const float halfW = _model_width * 0.5f;
    const float halfH = _model_height * 0.5f;

    const float turnEpsX = std::max(_model_width * 0.05f, dt * _pacman->getSpeed());
    const float turnEpsY = std::max(_model_height * 0.05f, dt * _pacman->getSpeed());

    constexpr float tiny = 1e-4f;

    // cell under current center (biased by current direction)
    auto center = getCenter(*_pacman);
    float cx = center.first, cy = center.second;
    biasByDir(_pacman->getDirection(), cx, cy, tiny);

    int col = colFromX(cx);
    int row = rowFromY(cy);

    const float cellLeft = xFromCol(col);
    const float cellTop = yFromRow(row);
    const float cellCx = cellLeft + halfW;
    const float cellCy = cellTop + halfH;

    // turn (no set allocation)
    if (_current_dir != _pacman->getDirection()) {
        const bool wantH = isHorizontal(_current_dir);
        const bool wantV = isVertical(_current_dir);

        const bool alignedH = std::fabs(center.second - cellCy) <= turnEpsY;
        const bool alignedV = std::fabs(center.first - cellCx) <= turnEpsX;

        auto targetFree = [&](const Direction d) {
            switch (d) {
            case Direction::Left:
                return !isBlockedForPacman(row, col - 1);
            case Direction::Right:
                return !isBlockedForPacman(row, col + 1);
            case Direction::Up:
                return !isBlockedForPacman(row - 1, col);
            case Direction::Down:
                return !isBlockedForPacman(row + 1, col);
            default:
                return false;
            }
        };

        if (targetFree(_current_dir)) {
            if (wantH && alignedH) {
                snapHorizontally(row);
                _pacman->setDirection(_current_dir);
            } else if (wantV && alignedV) {
                snapVertically(col);
                _pacman->setDirection(_current_dir);
            }
        }
    }

    const Direction dir = _pacman->getDirection();

    // recompute center after potential snap
    center = getCenter(*_pacman);
    const float realCx = center.first;
    const float realCy = center.second;

    float scx = realCx, scy = realCy;
    biasByDir(dir, scx, scy, tiny);

    col = colFromX(scx);
    row = rowFromY(scy);

    // viable directions for Pac-Man from this cell
    const std::set<Direction> viable = getAvailableDirectionsAt(row, col);

    // shared clamp + move flags (Pac-Man update now respects the flags)
    _pacman->applyGridConstraints(*this, row, col, viable, dt, _model_width, _model_height);
}

void World::update(const float dt) {
    if (!_pacman) return;

    if (_hit_cooldown > 0.f) {
        _hit_cooldown = std::max(0.f, _hit_cooldown - dt);
        return;
    }

    if (_score)
        _score->update(dt);

    updateGhostRelease(dt);

    if (_frightened_left > 0.f)
        _frightened_left = std::max(0.f, _frightened_left - dt);

    for (const auto& g : _ghosts)
        if (g) g->update(*this, dt);


    // The only role for this None direction is to make pacman stay at his location when spawned
    if (_current_dir != Direction::None) {
        handleCollision(dt);
        _pacman->update(*this, dt);
        collect();
    }

    ghostInteractions();
}

void World::eat(const std::shared_ptr<Collectable>& collectable) {
    const float cx = collectable->getPosition().first + _model_width * 0.5f;
    const float cy = collectable->getPosition().second + _model_height * 0.5f;

    const int col = std::clamp(colFromX(cx), 0, _cols - 1);
    const int row = std::clamp(rowFromY(cy), 0, _rows - 1);

    if (collectable_at[row][col] == collectable)
        collectable_at[row][col].reset();

    std::erase(_collectables, collectable);

    // This is used only for checking purposes, it's not the actual score gained for coins
    const int s = collectable->getScore();
    if (s == Constants::COIN_SCORE_FASTEST) {
        _score->onNotify(EventType::CoinEaten);
        _coins_left = std::max(0, _coins_left - 1);
    } else if (s == Constants::FRUIT_SCORE) {
        _score->onNotify(EventType::FruitEaten);
        startFrightened(_frightened_duration);
        _fruits_left = std::max(0, _fruits_left - 1);
    }
}

void World::ghostInteractions() {
    if (!_pacman)
        return;

    for (auto& ghost : _ghosts) {
        if (ghost && collides(*_pacman, *ghost)) {
            (ghost->getMode() == GhostMode::Fear) ? eatGhost(ghost) : loseLife();
            return;
        }
    }
}

void World::loseLife() {
    _hit_cooldown = Constants::HIT_COOLDOWN;
    --_lives_left;

    if (_lives_left <= 0) {
        _lives_left = 0;
        if (_score)
            _score->onNotify(EventType::Died);
        return;
    }

    // std::cout << "Life Lost. Lives left: " << _lives_left << "\n";

    _frightened_left = 0.f;
    _pacman->respawn(_pac_spawn_x, _pac_spawn_y);
    _current_dir = Direction::None;

    _level_time = 0.f;
    _released_ghost_3 = false;
    _released_ghost_4 = false;

    for (size_t i = 0; i < _ghosts.size(); ++i) {
        if (!_ghosts[i])
            continue;
        const GhostMode start = (i < 2) ? GhostMode::Chase : GhostMode::Center;
        _ghosts[i]->resetToSpawn(start);
        _ghosts[i]->forceEndFrightened(start);
    }
}

void World::eatGhost(const std::shared_ptr<Ghost>& ghost) {
    ghost->eaten();
    ++_ghosts_eaten_total;
    _score->onNotify(EventType::GhostEaten);
}

bool World::collides(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

bool World::collides(const EntityModel& m1, const EntityModel& m2) const {
    return collides(m1.getPosition().first, m1.getPosition().second, _model_width, _model_height,
                    m2.getPosition().first, m2.getPosition().second, _model_width, _model_height);
}

std::pair<float, float> World::getCenter(const EntityModel& model) const {
    return {model.getPosition().first + _model_width * 0.5f, model.getPosition().second + _model_height * 0.5f};
}

void World::setWallsTypes() const {
    static constexpr std::array kType = {
        WallType::Isolated,   // 0
        WallType::EndLeft,    // 1
        WallType::EndDown,    // 2
        WallType::CornerSW,   // 3
        WallType::EndRight,   // 4
        WallType::Horizontal, // 5
        WallType::CornerSE,   // 6
        WallType::T_S,        // 7
        WallType::EndUp,      // 8
        WallType::CornerNW,   // 9
        WallType::Vertical,   // 10
        WallType::T_W,        // 11
        WallType::CornerNE,   // 12
        WallType::T_N,        // 13
        WallType::T_E,        // 14
        WallType::Cross       // 15
    };

    auto hasWall = [&](const int r, const int c) { return inBounds(r, c, _rows, _cols) && wall_at[r][c] != nullptr; };

    for (int r = 0; r < _rows; ++r) {
        for (int c = 0; c < _cols; ++c) {
            auto& w = wall_at[r][c];
            if (!w)
                continue;

            const int mask = (hasWall(r - 1, c) << 3) | (hasWall(r, c + 1) << 2) | (hasWall(r + 1, c) << 1) |
                             (hasWall(r, c - 1) << 0);

            w->setWallType(kType[mask]);
        }
    }
}

void World::startFrightened(const float duration) {
    _frightened_left = std::max(0.f, duration);
    for (auto& g : _ghosts) {
        if (g)
            g->enterFrightened();
    }
}


std::set<Direction> World::getAvailableGhostDirectionsAt(const int row, const int col, const Ghost& ghost) const {
    std::set<Direction> dirs;

    auto can_step = [&](int fr, int fc, int tr, int tc) {
        if (isWallCell(tr, tc))
            return false;

        // =========== Keep Center-mode ghosts inside the house [START] ===========
        if (ghost.getMode() == GhostMode::Center) {
            return isHouseCell(tr, tc);
        }
        // =========== Keep Center-mode ghosts inside the house [END] ===========

        const bool from_house_side = isHouseCell(fr, fc) || isGateCell(fr, fc);
        const bool to_house_side = isHouseCell(tr, tc) || isGateCell(tr, tc);

        if (!from_house_side && to_house_side)
            return false;

        if (from_house_side && !to_house_side) {
            // Leaving the house area is only allowed through the gate cell (when not Center).
            if (!isGateCell(fr, fc))
                return false;
        }

        if (isGateCell(fr, fc) && isHouseCell(tr, tc) && ghost.getMode() != GhostMode::Center)
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

} // namespace logic