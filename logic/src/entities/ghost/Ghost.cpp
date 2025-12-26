//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/Ghost.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utils/Random.h>

#include <world/World.h>

namespace logic {

bool Ghost::atTileCenter(int row, int col, float eps_x, float eps_y) const {
    if (!_world)
        return false;

    const auto [center_x, center_y] = _world->getCenter(this);

    const float tile_w = _world->xFromCol(1) - _world->xFromCol(0);
    const float tile_h = _world->yFromRow(1) - _world->yFromRow(0);

    const float cell_left = _world->xFromCol(col);
    const float cell_top = _world->yFromRow(row);

    const float cell_cx = cell_left + tile_w * 0.5f;
    const float cell_cy = cell_top + tile_h * 0.5f;

    return std::fabs(center_x - cell_cx) <= eps_x && std::fabs(center_y - cell_cy) <= eps_y;
}

void Ghost::snapToRow(int row) {
    if (!_world)
        return;
    setPosition(getPosition().first, _world->yFromRow(row));
}

void Ghost::snapToCol(int col) {
    if (!_world)
        return;
    setPosition(_world->xFromCol(col), getPosition().second);
}

std::pair<int, int> Ghost::ghostCellFromCenterBias() const {
    if (!_world)
        return {0, 0};

    auto [center_x, center_y] = _world->getCenter(this);

    const float tiny = 1e-4f;
    switch (getDirection()) {
    case Direction::Left:
        center_x -= tiny;
        break;
    case Direction::Right:
        center_x += tiny;
        break;
    case Direction::Up:
        center_y -= tiny;
        break;
    case Direction::Down:
        center_y += tiny;
        break;
    default:
        break;
    }

    const int col = _world->colFromX(center_x);
    const int row = _world->rowFromY(center_y);
    return {row, col};
}

std::pair<int, int> Ghost::pacmanCellFromCenter() const {
    if (!_world)
        return {0, 0};

    const auto* pm = _world->getPacMan(); // assumes World::getPacMan() exists
    if (!pm)
        return {0, 0};

    const auto [pm_cx, pm_cy] = _world->getCenter(pm);
    const int pm_col = _world->colFromX(pm_cx);
    const int pm_row = _world->rowFromY(pm_cy);
    return {pm_row, pm_col};
}
int Ghost::manhattan(int r1, int c1, int r2, int c2) { return std::abs(r1 - r2) + std::abs(c1 - c2); }

int Ghost::dr(Direction d) { return (d == Direction::Up) ? -1 : (d == Direction::Down) ? 1 : 0; }

int Ghost::dc(Direction d) { return (d == Direction::Left) ? -1 : (d == Direction::Right) ? 1 : 0; }

Direction Ghost::opposite(Direction d) {
    switch (d) {
    case Direction::Left:
        return Direction::Right;
    case Direction::Right:
        return Direction::Left;
    case Direction::Up:
        return Direction::Down;
    case Direction::Down:
        return Direction::Up;
    default:
        return Direction::None;
    }
}


static constexpr float kFlashSeconds = 2.f; // last seconds -> white flashing

bool Ghost::isFrightened() const {
    // =========== One source of truth: world timer [START] ===========
    return _world && _world->getFrightenedLeft() > 0.f;
    // =========== One source of truth: world timer [END] ===========
}

void Ghost::enterFrightened(float /*duration*/) {
    // =========== Enter fear mode once (reverse + slow + notify view) [START] ===========
    if (_base_speed == 0.f)
        _base_speed = _speed;

    if (!_fear_active) {
        _mode_before_fear = _mode;
        setDirection(opposite(getDirection())); // immediate reverse (classic Pac-Man feel)
    }

    _mode = GhostMode::Fear;
    _speed = _base_speed * 0.6f;

    _fear_active = true;
    _flash_sent = false;

    notify(EventType::FrightenedStarted);
    // =========== Enter fear mode once (reverse + slow + notify view) [END] ===========
}

void Ghost::syncFrightenedFromWorld() {
    // =========== Sync local state with global frightened timer [START] ===========
    const float left = _world ? _world->getFrightenedLeft() : 0.f;
    const bool frightened = left > 0.f;

    if (frightened) {
        // If we were locked / spawned late, still snap into fear properly.
        if (!_fear_active)
            enterFrightened(left);

        // Last 2 seconds -> switch to flashing/white visuals.
        if (!_flash_sent && left <= kFlashSeconds) {
            _flash_sent = true;
            notify(EventType::FrightenedFlashingStarted);
        }
    } else {
        // World says it ended -> everybody ends together.
        if (_fear_active) {
            _fear_active = false;
            _flash_sent = false;

            _mode = _mode_before_fear;
            if (_base_speed != 0.f)
                _speed = _base_speed;

            notify(EventType::FrightenedEnded);
        }
    }
    // =========== Sync local state with global frightened timer [END] ===========
}

void Ghost::update(World* world, float dt) {
    // =========== World pointer setup [START] ===========
    if (_world == nullptr)
        _world = world;
    if (_world == nullptr)
        return;
    // =========== World pointer setup [END] ===========

    // =========== Frightened handling (NEW) [START] ===========
    // IMPORTANT: we only sync fear here. We do NOT touch your movement logic below.
    syncFrightenedFromWorld();
    // =========== Frightened handling (NEW) [END] ===========

    // =========== Figure out current tile (using center + tiny bias) [START] ===========
    auto [center_x, center_y] = _world->getCenter(this);
    constexpr float tiny = 1e-4f;

    switch (getDirection()) {
    case Direction::Left:
        center_x -= tiny;
        break;
    case Direction::Right:
        center_x += tiny;
        break;
    case Direction::Up:
        center_y -= tiny;
        break;
    case Direction::Down:
        center_y += tiny;
        break;
    default:
        break;
    }

    const int cur_col = _world->colFromX(center_x);
    const int cur_row = _world->rowFromY(center_y);
    // =========== Figure out current tile (using center + tiny bias) [END] ===========

    // =========== Movement constraints (walls + gate rules) [START] ===========
    const auto viable = _world->getAvailableGhostDirectionsAt(cur_row, cur_col, this);
    if (viable.empty())
        return;
    // =========== Movement constraints (walls + gate rules) [END] ===========

    // =========== Only turn when we're at tile center [START] ===========
    const float tile_w = _world->xFromCol(1) - _world->xFromCol(0);
    const float tile_h = _world->yFromRow(1) - _world->yFromRow(0);

    const float cell_left = _world->xFromCol(cur_col);
    const float cell_top = _world->yFromRow(cur_row);

    const float cell_cx = cell_left + tile_w * 0.5f;
    const float cell_cy = cell_top + tile_h * 0.5f;

    const auto [cx2, cy2] = _world->getCenter(this);

    const float eps_x = tile_w * 0.03f;
    const float eps_y = tile_h * 0.03f;

    const bool at_center = std::fabs(cx2 - cell_cx) <= eps_x && std::fabs(cy2 - cell_cy) <= eps_y;
    // =========== Only turn when we're at tile center [END] ===========

    // =========== Direction decision (house / center / fear / chase) [START] ===========
    if (at_center) {
        auto pick_random_from_vec = [&](const std::vector<Direction>& v) -> Direction {
            return v[Random::instance().uniformInt(0, static_cast<int>(v.size()) - 1)];
        };

        auto pick_random = [&](const std::set<Direction>& s) -> Direction {
            std::vector<Direction> v(s.begin(), s.end());
            return pick_random_from_vec(v);
        };

        auto is_house_side = [&](int r, int c) { return _world->isHouseCell(r, c) || _world->isGateCell(r, c); };

        auto next_cell = [&](Direction d) { return std::pair{cur_row + dr(d), cur_col + dc(d)}; };

        if (!viable.contains(getDirection())) {
            std::set<Direction> options = viable;

            if (options.size() > 1)
                options.erase(opposite(getDirection()));

            if (options.empty())
                options = viable;

            setDirection(pick_random(options));
        }

        if (_mode != GhostMode::Center && is_house_side(cur_row, cur_col)) {
            Direction chosen = getDirection();

            if (_world->isGateCell(cur_row, cur_col)) {
                bool found_out = false;
                constexpr Direction order[] = {Direction::Up, Direction::Left, Direction::Right, Direction::Down};

                for (Direction d : order) {
                    if (!viable.contains(d))
                        continue;
                    auto [nr, nc] = next_cell(d);
                    if (!is_house_side(nr, nc)) {
                        chosen = d;
                        found_out = true;
                        break;
                    }
                }

                if (!found_out)
                    chosen = pick_random(viable);

                setDirection(chosen);
            } else {
                int gate_row = -1, gate_col = -1;
                int best_gate_dist = std::numeric_limits<int>::max();

                for (int r = 0; r < _world->getRows(); ++r) {
                    for (int c = 0; c < _world->getCols(); ++c) {
                        if (!_world->isGateCell(r, c))
                            continue;

                        const int d = manhattan(cur_row, cur_col, r, c);
                        if (d < best_gate_dist) {
                            best_gate_dist = d;
                            gate_row = r;
                            gate_col = c;
                        }
                    }
                }

                if (gate_row != -1) {
                    int best_dist = std::numeric_limits<int>::max();
                    std::vector<Direction> best_dirs;

                    for (Direction d : viable) {
                        auto [nr, nc] = next_cell(d);
                        const int dist_to_gate = manhattan(nr, nc, gate_row, gate_col);

                        if (dist_to_gate < best_dist) {
                            best_dist = dist_to_gate;
                            best_dirs.clear();
                            best_dirs.push_back(d);
                        } else if (dist_to_gate == best_dist) {
                            best_dirs.push_back(d);
                        }
                    }

                    if (!best_dirs.empty())
                        setDirection(pick_random_from_vec(best_dirs));
                } else {
                    setDirection(pick_random(viable));
                }
            }
        } else if (_mode == GhostMode::Center) {
            std::set<Direction> options = viable;

            if (options.size() > 1)
                options.erase(opposite(getDirection()));

            if (options.empty())
                options = viable;

            if (options.size() > 1 && Random::instance().probability(0.35f))
                setDirection(pick_random(options));
        } else if (_mode == GhostMode::Fear) {
            const auto [pm_r, pm_c] = pacmanCellFromCenter();

            int best_dist = std::numeric_limits<int>::min();
            std::vector<Direction> best_dirs;

            for (Direction d : viable) {
                auto [nr, nc] = next_cell(d);
                const int dist = manhattan(nr, nc, pm_r, pm_c);

                if (dist > best_dist) {
                    best_dist = dist;
                    best_dirs.clear();
                    best_dirs.push_back(d);
                } else if (dist == best_dist) {
                    best_dirs.push_back(d);
                }
            }

            if (!best_dirs.empty())
                setDirection(pick_random_from_vec(best_dirs));
        } else {
            decideDirection();

            if (!viable.contains(getDirection())) {
                std::set<Direction> options = viable;

                if (options.size() > 1)
                    options.erase(opposite(getDirection()));

                if (options.empty())
                    options = viable;

                setDirection(pick_random(options));
            }
        }
    }
    // =========== Direction decision (house / center / fear / chase) [END] ===========

    // =========== Apply movement [START] ===========
    MovableEntityModel::update(dt);
    // =========== Apply movement [END] ===========
}

} // namespace logic
