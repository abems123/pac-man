//
// Created by abdellah on 12/11/25.
//

#pragma once

#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "entities/PacMan.h"
#include "score/Score.h"

class Collectable;

namespace logic {

class AbstractFactory;
class EntityModel;
class Ghost;
class Wall;

/**
 * @brief Owns the maze grid, entities and the main update loop.
 *
 * World parses the map, spawns entities through a factory, updates Pac-Man and ghosts,
 * handles collisions/turning, collectables, and global timers (like frightened mode).
 */
class World {
public:
    /** Builds a world for a level.
     *  @param factory Factory used to create entities.
     *  @param carryScore Score carried from previous level.
     *  @param carryLives Lives carried from previous level.
     *  @param level Current level number.
     */
    explicit World(const std::shared_ptr<AbstractFactory>& factory, int level, int carryScore, int carryLives);

    /**
     * @brief Computes the per-level frightened duration.
     */
    void applyFrightenedDifficulty();

    /**
     * @brief Update the world for one frame.
     * @param dt Delta time in seconds.
     */
    void update(float dt);

    /**
     * @brief Change the requested direction for Pac-Man (applied when turning becomes possible).
     * @param direction Desired direction.
     */
    void changePacmanDirection(Direction direction);

    /**
     * @brief Parse the map and create entities + lookup grids.
     */
    void parseMap();

    /**
     * @brief Assign wall types based on neighboring walls (for visuals).
     */
    void setWallsTypes() const;

    /**
     * @brief Collect items that overlap Pac-Man.
     */
    void collect();

    /**
     * @brief Consume a collectable: remove it and apply effects (score / frightened).
     * @param collectable The eaten collectable.
     */
    void eat(const std::shared_ptr<Collectable>& collectable);

    /**
     * @brief Eat a frightened ghost: respawn it and apply effects (score / return to spawn).
     */
    void ghostInteractions();

    void loseLife();

    void eatGhost(const std::shared_ptr<Ghost>& ghost);

    /** Returns how many lives Pac-Man still has.
     *  @return Number of lives left.
     */
    int getLivesLeft() const { return _lives_left; }

    /** Returns how many coins are still on the map.
     *  @return Number of coins not yet collected.
     */
    int getCoinsLeft() const { return _coins_left; }

    /** Returns how many fruits are still on the map.
     *  @return Number of fruits not yet collected.
     */
    int getFruitsLeft() const { return _fruits_left; }

    /**
     * @brief Handle turning + predictive wall blocking for Pac-Man.
     * @param dt Delta time in seconds.
     */
    void handleCollision(float dt) const;

    /**
     * @brief Check if a cell contains a wall (out-of-bounds counts as wall).
     * @param r Row index.
     * @param c Column index.
     * @return True if wall or out of bounds.
     */
    [[nodiscard]] bool isWallCell(int r, int c) const;

    /**
     * @brief Check if a cell is a gate tile ('&').
     * @param r Row index.
     * @param c Column index.
     * @return True if gate.
     */
    [[nodiscard]] bool isGateCell(int r, int c) const;

    /**
     * @brief Check if a cell is inside the ghost house interior (not the gate).
     * @param r Row index.
     * @param c Column index.
     * @return True if inside house.
     */
    [[nodiscard]] bool isHouseCell(int r, int c) const;

    /**
     * @brief Directions Pac-Man may take from a cell (walls + gate/house are blocked).
     * @param row Row index.
     * @param col Column index.
     * @return Set of allowed directions.
     */
    [[nodiscard]] std::set<Direction> getAvailableDirectionsAt(int row, int col) const;

    /**
     * @brief Directions a ghost may take from a cell (walls + gate/house rules).
     * @param row Row index.
     * @param col Column index.
     * @param ghost Ghost pointer (needed for mode rules).
     * @return Set of allowed directions.
     */
    [[nodiscard]] std::set<Direction> getAvailableGhostDirectionsAt(int row, int col, const Ghost* ghost) const;

    /**
     * @brief Convert world X to column index.
     * @param x World x coordinate.
     * @return Column index.
     */
    [[nodiscard]] int colFromX(float x) const { return static_cast<int>((x + 1.0f) / _model_width); }

    /**
     * @brief Convert world Y to row index.
     * @param y World y coordinate.
     * @return Row index.
     */
    [[nodiscard]] int rowFromY(float y) const { return static_cast<int>((y + 1.0f) / _model_height); }

    /**
     * @brief Convert column index to tile top-left world X.
     * @param c Column index.
     * @return World X.
     */
    [[nodiscard]] float xFromCol(int c) const { return -1.f + c * _model_width; }

    /**
     * @brief Convert row index to tile top-left world Y.
     * @param r Row index.
     * @return World Y.
     */
    [[nodiscard]] float yFromRow(int r) const { return -1.f + r * _model_height; }

    /**
     * @brief Get model center (top-left + half tile).
     * @param model Model pointer.
     * @return (center_x, center_y).
     */
    [[nodiscard]] std::pair<float, float> getCenter(const EntityModel* model) const;

    /**
     * @brief Snap Pac-Man's X to a column (used for clean turns).
     * @param x Column index.
     */
    void snapVertically(int x) const;

    /**
     * @brief Snap Pac-Man's Y to a row (used for clean turns).
     * @param y Row index.
     */
    void snapHorizontally(int y) const;

    /**
     * @brief AABB overlap check for two rectangles.
     * @param ax Rect A x.
     * @param ay Rect A y.
     * @param aw Rect A w.
     * @param ah Rect A h.
     * @param bx Rect B x.
     * @param by Rect B y.
     * @param bw Rect B w.
     * @param bh Rect B h.
     * @return True if overlapped.
     */
    bool collides(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const;

    /**
     * @brief AABB overlap check for two models using tile-sized hitboxes.
     * @param m1 First model.
     * @param m2 Second model.
     * @return True if overlapped.
     */
    bool collides(const EntityModel* m1, const EntityModel* m2) const;

    /**
     * @brief Start or extend frightened mode globally.
     * @param duration Frightened duration in seconds (timer only increases, never shrinks).
     */
    void startFrightened(float duration);

    /**
     * @brief Remaining frightened seconds.
     * @return Seconds left (0 if inactive).
     */
    [[nodiscard]] float getFrightenedLeft() const { return _frightened_left; }

    /**
     * @brief Get Pac-Man pointer (can be null if not spawned).
     * @return Pac-Man pointer.
     */
    [[nodiscard]] const PacMan* getPacMan() const { return _pacman.get(); }

    /**
     * @brief Get score system.
     * @return Score pointer.
     */
    [[nodiscard]] Score* getScore() const { return _score.get(); }

    /**
     * @brief Get ghosts list.
     * @return Reference to ghosts vector.
     */
    [[nodiscard]] const std::vector<std::shared_ptr<Ghost>>& getGhosts() const { return _ghosts; }

    /**
     * @brief Get gate cells list.
     * @return Gate coordinates.
     */
    [[nodiscard]] const std::vector<std::pair<int, int>>& getGateCells() const { return _gate_cells; }

    /**
     * @brief Get map rows.
     * @return Number of rows.
     */
    [[nodiscard]] int getRows() const { return _rows; }

    /**
     * @brief Get map cols.
     * @return Number of cols.
     */
    [[nodiscard]] int getCols() const { return _cols; }

    int getGhostsEatenTotal() const { return _ghosts_eaten_total; }

private:
    int _ghosts_eaten_total{0};

    /** Applies level-based speed scaling to all ghosts. */
    void applyGhostDifficulty();

    /**
     * @brief Update delayed release logic for some ghosts.
     * @param dt Delta time in seconds.
     */
    void updateGhostRelease(float dt);

    /**
     * @brief Pac-Man blocked rule: wall OR gate OR house.
     * @param r Row index.
     * @param c Column index.
     * @return True if Pac-Man cannot enter.
     */
    [[nodiscard]] bool isBlockedForPacman(int r, int c) const;

    float _hit_cooldown{0.f};
    int _lives_left{3};
    float _pac_spawn_x{0.f}, _pac_spawn_y{0.f};

    /** @brief Score manager. */
    std::unique_ptr<Score> _score;

    /** @brief Factory used to create entities. */
    std::shared_ptr<AbstractFactory> _factory;

    /** @brief Raw map loaded from ResourceManager. */
    std::vector<std::string> _map;

    /** @brief Map rows. */
    const int _rows;

    /** @brief Map cols. */
    const int _cols;

    /** @brief Tile width in world coords. */
    float _model_width;

    /** @brief Tile height in world coords. */
    float _model_height;

    /** @brief Global frightened time remaining (seconds). */
    float _frightened_left = 0.f;

    /** @brief Default frightened duration used when fruit is eaten. */
    float _frightened_duration;

    /** @brief Next movement direction requested by the player. */
    Direction _current_dir = Direction::None;

    /** @brief All wall entities. */
    std::vector<std::shared_ptr<Wall>> _walls{};

    /** @brief All collectables still present. */
    std::vector<std::shared_ptr<Collectable>> _collectables{};

    /** @brief Pac-Man entity. */
    std::shared_ptr<PacMan> _pacman{};

    /** @brief Ghost entities. */
    std::vector<std::shared_ptr<Ghost>> _ghosts{};

    /** @brief Wall lookup grid. */
    std::vector<std::vector<std::shared_ptr<Wall>>> wall_at;

    /** @brief Collectable lookup grid. */
    std::vector<std::vector<std::shared_ptr<Collectable>>> collectable_at;

    /** @brief True at gate tiles ('&'). */
    std::vector<std::vector<bool>> gate_at;

    /** @brief True at house interior tiles (computed). */
    std::vector<std::vector<bool>> house_at;

    /** @brief Gate tile coordinates. */
    std::vector<std::pair<int, int>> _gate_cells;

    /** @brief Seconds since level start. */
    float _level_time = 0.f;

    /** @brief Release guard for ghost index 2. */
    bool _released_ghost_3 = false;

    /** @brief Release guard for ghost index 3. */
    bool _released_ghost_4 = false;

    int _coins_left = 0;

    int _fruits_left = 0;

    int _level{1};
};

} // namespace logic