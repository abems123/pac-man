//
// Created by abdellah on 12/11/25.
//

#pragma once

#include "entities/MovableEntityModel.h"

#include <optional>

namespace logic {
class PacMan;
class World;

class Ghost : public MovableEntityModel {
protected:
    /** @return The world this ghost belongs to. Throws if not set yet. */
    World& world();

    /** @return The world this ghost belongs to. Throws if not set yet. */
    const World& world() const;

    /** Non-owning reference to the active world (set on first update). */
    std::optional<std::reference_wrapper<World>> _world;

    /** @brief Time left (seconds) before switching from Center to Chase. 0 = already released. */
    float _release_left = 0.f;

    /** Target position in world coords (used by chase logic). */
    double _target_x = 0.0;

    /** Target position in world coords (used by chase logic). */
    double _target_y = 0.0;

    /** Spawn/home position in world coords (used for resets/returning). */
    double _spawn_x = 0.0;

    /** Spawn/home position in world coords (used for resets/returning). */
    double _spawn_y = 0.0;

    /** Current ghost mode (center/chase/fear/dead). */
    GhostMode _mode = GhostMode::Center;

    float _base_speed = 0.f;

    GhostMode _mode_before_fear = GhostMode::Chase;

    /** Cached possible directions from the current tile (optional helper). */
    std::vector<Direction> available_directions;

    /** Manhattan distance between two grid cells. */
    static int manhattan(int r1, int c1, int r2, int c2);

    /** Row delta for a direction (Up=-1, Down=+1, else 0). */
    static int dr(Direction d);

    /** Col delta for a direction (Left=-1, Right=+1, else 0). */
    static int dc(Direction d);

    /** Returns the opposite direction. */
    static Direction opposite(Direction d);

    /**
     * Checks if the ghost center is close to the center of (row,col).
     * @param row Tile row.
     * @param col Tile col.
     * @param eps_x Allowed x error (world units).
     * @param eps_y Allowed y error (world units).
     * @return true if close enough to tile center.
     */
    [[nodiscard]] bool atTileCenter(int row, int col, float eps_x, float eps_y) const;

    /**
     * Snaps the ghost on the given row (keeps x, aligns y to tile top-left).
     * @param row Tile row to align to.
     */
    void snapToRow(int row);

    /**
     * Snaps the ghost on the given col (keeps y, aligns x to tile top-left).
     * @param col Tile col to align to.
     */
    void snapToCol(int col);

    /**
     * Gets the current (row,col) from the ghost center with a tiny bias
     * so it doesn't flip when sitting exactly on borders.
     * @return {row, col}.
     */
    [[nodiscard]] std::pair<int, int> ghostCellFromCenterBias() const;

    /**
     * Gets Pac-Man's current tile cell from his center.
     * @return {row, col}. Returns {0,0} if Pac-Man is missing.
     */
    [[nodiscard]] std::pair<int, int> pacmanCellFromCenter() const;

    /** Computes the current target for this ghost (depends on ghost type). */
    virtual void computeTarget(const World& world) = 0;

    /** Chooses a direction when appropriate (usually only at tile center). */
    virtual void decideDirection(const World& world) = 0;

public:
    Ghost(float x, float y, EntityType type);

    /** Resets this ghost back to its spawn position and starting mode.
     *  @param start The mode the ghost should start in after the reset.
     */
    void resetToSpawn(GhostMode start);

    /**
     * @brief Enter frightened mode locally (world owns the timer).
     */
    void enterFrightened();

    /**
     * @brief Check if frightened is active (global timer).
     * @return True if the world frightened timer is > 0.
     */
    bool isFrightened() const;

    /**
     * Sets the current behavior mode.
     * @param mode New mode.
     */
    void setMode(GhostMode mode);

    /**
     * @return Current ghost mode.
     */
    [[nodiscard]] GhostMode getMode() const { return _mode; }

    /** Adds a direction to available_directions (if you want to cache them). */
    void addAvailableDir(Direction dir) { available_directions.push_back(dir); }

    /** Clears available_directions. */
    void clearDirections() { available_directions.clear(); }

    /**
     * @brief Sets how long this ghost stays in Center mode before it starts chasing.
     * @param seconds Delay in seconds (0 means chase immediately).
     */
    void setReleaseDelay(float seconds) { _release_left = std::max(0.f, seconds); }

    /**
     * Updates frightened timer, calls decideDirection(), then moves.
     * @param world World pointer (stored once).
     * @param dt Delta time (seconds).
     */
    void update(World& world, float dt) override;

    /** Respawn the ghost after Pac-Man eats it. */
    void eaten();

    /** Sets the normal (non-frightened) speed used as base for fear slowdowns.
     *  @param speed New base speed.
     */
    void setBaseSpeed(float speed);

    /**
     * @brief Force the ghost out of frightened visuals immediately.
     *
     * Used when Pac-Man dies and the level resets, so ghosts do not keep frightened frames
     * while chasing during the hit cooldown.
     *
     * @param newMode Mode to apply after clearing frightened state.
     */
    void forceEndFrightened(GhostMode newMode);

private:
    /** True once the spawn position has been stored from the initial placement. */
    bool _spawn_set{false};

    /** When true, this ghost ignores frightened mode until the world's frightened timer reaches 0. */
    bool _fear_locked_out{false};

    /** Store the current position as spawn point if it has not been stored yet. */
    void ensureSpawnStored();

    /**
     * @brief Sync local mode/speed + send flashing/end events based on world timer.
     */
    void syncFrightenedFromWorld();

    /** @brief Seconds at the end of frightened where ghosts flash white. */
    float k_flash_seconds = 0.f;

    /** @brief True while this ghost is considered in fear mode locally. */
    bool _fear_active = false;

    /** @brief True once we already told the view to start flashing this cycle. */
    bool _flash_sent = false;
};

} // namespace logic
