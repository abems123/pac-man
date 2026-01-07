//
// Created by abdellah on 12/13/25.
//

#include "entities/MovableEntityModel.h"

#include <cmath>
#include <events/EventType.h>
#include <world/World.h>

namespace logic {
MovableEntityModel::MovableEntityModel(const float x, const float y, const EntityType type, const float speed)
    : EntityModel(x, y, type), _speed(speed) {}

void MovableEntityModel::setDirection(const Direction direction) {
    if (_direction == direction)
        return;

    _direction = direction;

    switch (direction) {
    case Direction::Left:
        notify(EventType::DirectionChangedLeft);
        break;
    case Direction::Up:
        notify(EventType::DirectionChangedUp);
        break;
    case Direction::Down:
        notify(EventType::DirectionChangedDown);
        break;
    case Direction::Right:
        notify(EventType::DirectionChangedRight);
        break;
    default:
        break;
    }
}
void MovableEntityModel::setMove(const Direction dir, const bool can_move) {
    if (dir == Direction::Right) {
        _can_move_right = can_move;
    } else if (dir == Direction::Left) {
        _can_move_left = can_move;
    } else if (dir == Direction::Up) {
        _can_move_up = can_move;
    } else if (dir == Direction::Down) {
        _can_move_down = can_move;
    }
}
void MovableEntityModel::update(World& world, float dt) {
    // =========== Respect collision flags [START] ===========
    const bool allowed = (_direction == Direction::Left && _can_move_left) ||
                         (_direction == Direction::Right && _can_move_right) ||
                         (_direction == Direction::Up && _can_move_up) ||
                         (_direction == Direction::Down && _can_move_down);

    if (_direction == Direction::None || !allowed)
        return;
    // =========== Respect collision flags [END] ===========

    // =========== Move and notify only if we actually moved [START] ===========
    const float oldX = getPosition().first;
    const float oldY = getPosition().second;

    const float dist = _speed * dt;

    const float dx = (_direction == Direction::Left ? -1.f : _direction == Direction::Right ? 1.f : 0.f) * dist;
    const float dy = (_direction == Direction::Up ? -1.f : _direction == Direction::Down ? 1.f : 0.f) * dist;

    setPosition(oldX + dx, oldY + dy);

    const float newX = getPosition().first;
    const float newY = getPosition().second;

    constexpr float eps = 1e-6f;
    if (std::fabs(newX - oldX) > eps || std::fabs(newY - oldY) > eps)
        notify(EventType::Moved);
    // =========== Move and notify only if we actually moved [END] ===========
}

bool MovableEntityModel::applyGridConstraints(const World& world,
                                             const int row,
                                             const int col,
                                             const std::set<Direction>& viable,
                                             const float dt,
                                             const float model_w,
                                             const float model_h,
                                             const float center_eps_ratio) {
    // =========== Cell bounds + cell center [START] ===========
    const float halfW = model_w * 0.5f;
    const float halfH = model_h * 0.5f;

    const float left = world.xFromCol(col);
    const float top = world.yFromRow(row);
    const float right = left + model_w;
    const float bottom = top + model_h;

    const float cell_cx = left + halfW;
    const float cell_cy = top + halfH;
    // =========== Cell bounds + cell center [END] ===========

    // =========== Current center + step [START] ===========
    const auto [px, py] = getPosition(); // top-left
    const float cx = px + halfW;
    const float cy = py + halfH;

    const float step = dt * _speed;
    // =========== Current center + step [END] ===========

    // =========== Move flags (allowed OR still inside current cell) [START] ===========
    setMove(Direction::Left, viable.contains(Direction::Left) || (cx - halfW) > left);
    setMove(Direction::Right, viable.contains(Direction::Right) || (cx + halfW) < right);
    setMove(Direction::Up, viable.contains(Direction::Up) || (cy - halfH) > top);
    setMove(Direction::Down, viable.contains(Direction::Down) || (cy + halfH) < bottom);
    // =========== Move flags (allowed OR still inside current cell) [END] ===========

    // =========== Clamp if next step would cross a wall [START] ===========
    switch (_direction) {
    case Direction::Left: {
        const float nextCx = cx - step;
        if (!viable.contains(Direction::Left) && (nextCx - halfW) < left) {
            setMove(Direction::Left, false);
            setPosition(left, py);
        }
    } break;

    case Direction::Right: {
        const float nextCx = cx + step;
        if (!viable.contains(Direction::Right) && (nextCx + halfW) > right) {
            setMove(Direction::Right, false);
            setPosition(right - model_w, py);
        }
    } break;

    case Direction::Up: {
        const float nextCy = cy - step;
        if (!viable.contains(Direction::Up) && (nextCy - halfH) < top) {
            setMove(Direction::Up, false);
            setPosition(px, top);
        }
    } break;

    case Direction::Down: {
        const float nextCy = cy + step;
        if (!viable.contains(Direction::Down) && (nextCy + halfH) > bottom) {
            setMove(Direction::Down, false);
            setPosition(px, bottom - model_h);
        }
    } break;

    default:
        break;
    }
    // =========== Clamp if next step would cross a wall [END] ===========

    // =========== "At center" check (after clamp) [START] ===========
    const auto [px2, py2] = getPosition();
    const float cx2 = px2 + halfW;
    const float cy2 = py2 + halfH;

    const float eps_x = model_w * center_eps_ratio;
    const float eps_y = model_h * center_eps_ratio;

    return std::fabs(cx2 - cell_cx) <= eps_x && std::fabs(cy2 - cell_cy) <= eps_y;
    // =========== "At center" check (after clamp) [END] ===========
}

} // namespace logic
