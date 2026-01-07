//
// Created by abdellah on 12/11/25.
//

#include "../../../include/entities/ghost/Ghost.h"

#include <algorithm>
#include <cmath>
#include <events/EventType.h>
#include <utils/Constants.h>
#include <utils/Random.h>

#include <world/World.h>

namespace logic {

World& Ghost::world() {
    if (!_world)
        throw std::logic_error("Ghost::world() called before world was set.");
    return _world->get();
}

const World& Ghost::world() const {
    if (!_world)
        throw std::logic_error("Ghost::world() called before world was set.");
    return _world->get();
}

bool Ghost::atTileCenter(int row, int col, float eps_x, float eps_y) const {
    const World& w = world();
    const auto [center_x, center_y] = w.getCenter(*this);

    const float tile_w = w.xFromCol(1) - w.xFromCol(0);
    const float tile_h = w.yFromRow(1) - w.yFromRow(0);

    const float cell_left = w.xFromCol(col);
    const float cell_top  = w.yFromRow(row);

    const float cell_cx = cell_left + tile_w * 0.5f;
    const float cell_cy = cell_top  + tile_h * 0.5f;

    return std::fabs(center_x - cell_cx) <= eps_x && std::fabs(center_y - cell_cy) <= eps_y;
}


void Ghost::snapToRow(int row) {
    setPosition(getPosition().first, world().yFromRow(row));
}

void Ghost::snapToCol(int col) {
    setPosition(world().xFromCol(col), getPosition().second);
}


std::pair<int, int> Ghost::ghostCellFromCenterBias() const {
    const World& w = world();
    auto [center_x, center_y] = w.getCenter(*this);

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

    const int col = w.colFromX(center_x);
    const int row = w.rowFromY(center_y);
    return {row, col};
}

std::pair<int, int> Ghost::pacmanCellFromCenter() const {
    const World& w = world();

    const auto& pm = w.getPacMan();
    if (!pm)
        return {0, 0};

    const auto [pm_cx, pm_cy] = w.getCenter(*pm);
    return {w.rowFromY(pm_cy), w.colFromX(pm_cx)};
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

bool Ghost::isFrightened() const {
    return world().getFrightenedLeft() > 0.f;
}

void Ghost::setMode(const GhostMode mode) {
    // =========== If we’re frightened, just remember what to return to [START] ===========
    if (_mode == GhostMode::Fear) {
        _mode_before_fear = mode;
        return;
    }
    // =========== If we’re not frightened, apply normally [END] ===========
    _mode = mode;
}

Ghost::Ghost(const float x, const float y, const EntityType type)
    : MovableEntityModel(x, y, type, Constants::GHOST_START_SPEED) {
    _speed = Constants::GHOST_START_SPEED;
    if (!_spawn_set) {
        _spawn_x = getPosition().first;
        _spawn_y = getPosition().second;
        _spawn_set = true;
    }
}
void Ghost::enterFrightened() {
    const World& w = this->world();

    // =========== Enter fear mode once (reverse + slow + notify view) [START] ===========
    if (_base_speed == 0.f)
        _base_speed = _speed;

    // New fruit should override eaten lockout so all ghosts participate again.
    _fear_locked_out = false;

    // Only capture "mode before fear" the first time we enter fear.
    if (!_fear_active)
        _mode_before_fear = _mode;

    // Restart effect: reverse direction every time a fruit is eaten.
    setDirection(opposite(getDirection()));

    _mode = GhostMode::Fear;
    _speed = _base_speed * Constants::FRIGHTENED_MODE_SLOW_PERCENTAGE;

    _fear_active = true;
    _flash_sent = false;

    const float duration = w.getFrightenedLeft();
    k_flash_seconds = duration * Constants::FLASHING_PERCENTAGE;

    notify(EventType::FrightenedStarted);
    // =========== Enter fear mode once (reverse + slow + notify view) [END] ===========
}

void Ghost::ensureSpawnStored() {
    // =========== Store spawn position once [START] ===========
    if (!_spawn_set) {
        _spawn_x = getPosition().first;
        _spawn_y = getPosition().second;
        _spawn_set = true;
    }
    // =========== Store spawn position once [END] ===========
}

void Ghost::syncFrightenedFromWorld() {
    // =========== Sync local state with global frightened timer [START] ===========
    const World& w = this->world();

    // =========== Respect fear lockout after being eaten [START] ===========
    const float left = _world ? w.getFrightenedLeft() : 0.f;

    if (_fear_locked_out) {
        if (left <= 0.f)
            _fear_locked_out = false;
        return;
    }
    // =========== Respect fear lockout after being eaten [END] ===========

    const bool frightened = left > 0.f;

    if (frightened) {
        // If we were locked / spawned late, still snap into fear properly.
        if (!_fear_active)
            enterFrightened();

        // Last (FrightenedDuration*Constants::FLASHING_PERCENTAGE) seconds -> switch to white visuals.
        if (!_flash_sent && left <= k_flash_seconds) {
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

void Ghost::update(World& world, float dt) {
    if (!_world)
        _world = world;

    World& w = this->world();

    syncFrightenedFromWorld();

    // =========== Current tile (center + tiny bias) [START] ===========
    auto [row, col] = ghostCellFromCenterBias();

    row = std::clamp(row, 0, w.getRows() - 1);
    col = std::clamp(col, 0, w.getCols() - 1);
    // =========== Current tile (center + tiny bias) [END] ===========

    const auto viable = w.getAvailableGhostDirectionsAt(row, col, *this);
    if (viable.empty())
        return;

    const float tile_w = w.xFromCol(1) - w.xFromCol(0);
    const float tile_h = w.yFromRow(1) - w.yFromRow(0);

    // =========== Shared collision + "at center" check [START] ===========
    const bool at_center = applyGridConstraints(*_world, row, col, viable, dt, tile_w, tile_h);
    // =========== Shared collision + "at center" check [END] ===========

    if (at_center) {
        // =========== Small helpers [START] ===========
        auto pick_random = [&](const std::set<Direction>& s) -> Direction {
            const int idx = Random::instance().uniformInt(0, static_cast<int>(s.size()) - 1);
            auto it = s.begin();
            for (int i = 0; i < idx; ++i)
                ++it;
            return *it;
        };

        auto pick_random_from_vec = [&](const std::vector<Direction>& v) -> Direction {
            return v[Random::instance().uniformInt(0, static_cast<int>(v.size()) - 1)];
        };

        auto pick_random_avoid = [&](const std::set<Direction>& s, const Direction banned) -> Direction {
            if (s.size() <= 1)
                return *s.begin();

            const int pick = Random::instance().uniformInt(0, static_cast<int>(s.size()) - 2);
            int i = 0;
            for (const Direction d : s) {
                if (d == banned)
                    continue;
                if (i++ == pick)
                    return d;
            }
            return pick_random(s);
        };

        auto pick_no_reverse = [&]() -> Direction {
            const Direction opp = opposite(getDirection());
            return (viable.size() > 1 && viable.contains(opp)) ? pick_random_avoid(viable, opp) : pick_random(viable);
        };

        auto ensure_valid_direction = [&]() {
            if (!viable.contains(getDirection()))
                setDirection(pick_no_reverse());
        };

        auto is_house_side = [&](int r, int c) { return w.isHouseCell(r, c) || w.isGateCell(r, c); };
        auto next_cell = [&](Direction d) { return std::pair{row + dr(d), col + dc(d)}; };
        // =========== Small helpers [END] ===========

        ensure_valid_direction();

        // =========== Direction decision (house / center / fear / chase) [START] ===========
        if (_mode != GhostMode::Center && is_house_side(row, col)) {
            if (w.isGateCell(row, col)) {
                Direction chosen = getDirection();
                bool found_out = false;

                constexpr Direction order[] = {Direction::Up, Direction::Left, Direction::Right, Direction::Down};
                for (const Direction d : order) {
                    if (!viable.contains(d))
                        continue;
                    const auto [nr, nc] = next_cell(d);
                    if (!is_house_side(nr, nc)) {
                        chosen = d;
                        found_out = true;
                        break;
                    }
                }

                setDirection(found_out ? chosen : pick_random(viable));
            } else {
                int gate_row = -1, gate_col = -1;
                int best_gate_dist = std::numeric_limits<int>::max();

                for (int r = 0; r < w.getRows(); ++r) {
                    for (int c = 0; c < w.getCols(); ++c) {
                        if (!w.isGateCell(r, c))
                            continue;
                        const int d = manhattan(row, col, r, c);
                        if (d < best_gate_dist) {
                            best_gate_dist = d;
                            gate_row = r;
                            gate_col = c;
                        }
                    }
                }

                if (gate_row == -1) {
                    setDirection(pick_random(viable));
                } else {
                    int best_dist = std::numeric_limits<int>::max();
                    std::vector<Direction> best_dirs;

                    for (const Direction d : viable) {
                        const auto [nr, nc] = next_cell(d);
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
                }
            }
        } else if (_mode == GhostMode::Center) {
            if (viable.size() > 1 && Random::instance().probability(0.35f))
                setDirection(pick_no_reverse());
        } else if (_mode == GhostMode::Fear) {
            const auto [pm_r, pm_c] = pacmanCellFromCenter();

            int best_dist = std::numeric_limits<int>::min();
            std::vector<Direction> best_dirs;

            for (const Direction d : viable) {
                const auto [nr, nc] = next_cell(d);
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
            decideDirection(world);
            ensure_valid_direction();
        }
        // =========== Direction decision (house / center / fear / chase) [END] ===========
    }

    MovableEntityModel::update(w, dt);
}

void Ghost::eaten() {
    // =========== Respawn + restore normal visuals immediately [START] ===========
    ensureSpawnStored();

    _fear_active = false;
    _flash_sent = false;

    // Force chase now
    _mode = GhostMode::Chase;
    _mode_before_fear = GhostMode::Chase;

    if (_base_speed != 0.f)
        _speed = _base_speed;

    // Keep chasing even if the global frightened timer is still running.
    _fear_locked_out = true;

    // Teleport back into the house.
    setPosition(_spawn_x, _spawn_y);

    // If direction is None, your switch hits default and keeps old frames.
    setDirection(Direction::Up);

    notify(EventType::FrightenedEnded);
    notify(EventType::Moved);
    // =========== Respawn + restore normal visuals immediately [END] ===========
}
void Ghost::setBaseSpeed(float speed) {
    _base_speed = speed;
    _speed = speed;
}
void Ghost::forceEndFrightened(const GhostMode newMode) {
    _fear_locked_out = false;
    _fear_active = false;
    _flash_sent = false;

    _mode_before_fear = newMode;
    _mode = newMode;

    if (_base_speed != 0.f)
        _speed = _base_speed;

    notify(EventType::FrightenedEnded);
    notify(EventType::Moved);
}

void Ghost::resetToSpawn(GhostMode start) {
    setPosition(_spawn_x, _spawn_y);
    _mode = start;

    if (_base_speed <= 0.f)
        _base_speed = Constants::GHOST_START_SPEED;

    _speed = _base_speed;

    _fear_active = false;
    _flash_sent = false;
}

} // namespace logic
