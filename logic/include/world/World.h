//
// Created by abdellah on 12/11/25.
//

#pragma once

#include <memory>
#include <vector>

#include "entities/EntityModel.h"
#include "entities/PacMan.h"

#include <map>
#include <optional>
#include <set>
#include <utils/ResourceManager.h>

class Collectable;

namespace representation {
class EntityView;
}

namespace logic {
class LockedGhost;
class AbstractFactory;
class Event;
class EntityModel;
class Stopwatch;
class Score;
class Fruit;
class Coin;
class Ghost;
class Wall;

class World {
    /** @brief True for tiles that are ghost-house gates ('&'). */
    std::vector<std::vector<bool>> gate_at;

    /** @brief True for tiles that belong to the ghost house interior. */
    std::vector<std::vector<bool>> house_at;

    /** @brief All gate tile coordinates ('&') in the map. Used for house-exit logic. */
    std::vector<std::pair<int,int>> _gate_cells;

    /** @brief Ghost spawn cells (used to find the house interior). */
    std::vector<std::pair<int, int>> ghost_spawns;

    /** @brief Marks all house tiles by flood-filling from ghost spawn tiles. */
    void computeHouseRegion();

    /** @brief Pac-Man cannot enter gate or house; this is the "blocked for Pac-Man" rule. */
    [[nodiscard]] bool isBlockedForPacman(int r, int c) const;

    /** @brief All wall entities present in the world. */
    std::vector<std::shared_ptr<Wall>> _walls{};

    /** @brief All collectable entities (coins, fruits) currently in the world. */
    std::vector<std::shared_ptr<Collectable>> _collectables{};

    /** @brief The Pac-Man entity controlled by the player. */
    std::shared_ptr<PacMan> _pacman{};

    /** @brief All ghost entities active in the world. */
    std::vector<std::shared_ptr<Ghost>> _ghosts{};

    /** @brief Manages the player's score and score-related events. */
    std::unique_ptr<Score> _score;

    /** @brief Factory used to create all world entities. */
    std::shared_ptr<AbstractFactory> _factory;

    /** @brief Raw map representation loaded from the map file.
     *
     * Each string represents one row of the maze, where
     * each character corresponds to a tile type (wall, coin, empty, etc.).
     */
    std::vector<std::string> _map;

    /** @brief Number of rows in the map.
     *
     * Derived from map.size().
     */
    const int _rows;

    /** @brief Number of columns in the map.
     *
     * Derived from the length of the first row (map.front().size()).
     */
    const int _cols;

    /** @brief Stores the next movement direction requested by the player.
     *
     * When the player presses one of the movement keys (W, A, S, D), the
     * corresponding direction is stored in this variable. Once Pac-Man
     * reaches an intersection, the game checks whether movement in this
     * direction is possible. If so, Pac-Man changes direction and
     * next_move_direction is cleared. */
    Direction _current_dir = Direction::None;

    /** @brief Width of a single tile in world coordinates. */
    float _model_width = 0.f;
    /** @brief Height of a single tile in world coordinates. */
    float _model_height = 0.f;

    /**
     * @brief Grid lookup table mapping wall positions to their entities.
     *
     * Allows constant-time checks for wall presence at a given
     * (row, column) position.
     */
    std::vector<std::vector<std::shared_ptr<Wall>>> wall_at;

    std::vector<std::vector<std::shared_ptr<Collectable>>> collectable_at;

    /** @brief Number of remaining lives for the player. */
    int _lives_left = 3;

    /** @brief Seconds since the level started */
    float _level_time = 0.f;
    bool _released_ghost_3 = false; // ghost index 2
    bool _released_ghost_4 = false; // ghost index 3

    /**
     * @brief Releases the delayed ghosts: after 5s and 10s they switch to Chase.
     * @param dt Time step in seconds.
     */
    void updateGhostRelease(float dt);

public:
    /**
     * @brief Constructs the game world and initializes all entities.
     *
     * Loads the map, initializes grid dimensions and collision data,
     * creates entities using the given factory, and assigns wall types.
     */
    explicit World(const std::shared_ptr<AbstractFactory>& factory);

    /** @brief Returns all gate tile positions ('&'). */
    [[nodiscard]] const std::vector<std::pair<int,int>>& getGateCells() const { return _gate_cells; }

    /**
     * @brief Returns Pac-Man (nullptr if not spawned).
     */
    [[nodiscard]] const PacMan* getPacMan() const { return _pacman.get(); };

    /**
     * @brief Directions a ghost may take from (row,col), taking walls + gate rules into account.
     *
     * Ghosts may enter the house only when dead, or when Pac-Man is inside.
     */
    [[nodiscard]] std::set<Direction> getAvailableGhostDirectionsAt(int row, int col, const Ghost* ghost) const;

    /**
     * @brief Converts world X to grid column index.
     */
    [[nodiscard]] int colFromX(float x) const;

    /**
     * @brief Converts world Y to grid row index.
     */
    [[nodiscard]] int rowFromY(float y) const;

    /**
     * @brief Converts a grid column to the world X of the tile's top-left.
     */
    [[nodiscard]] float xFromCol(int c) const;

    /**
     * @brief Converts a grid row to the world Y of the tile's top-left.
     */
    [[nodiscard]] float yFromRow(int r) const;

    /**
     * @brief Returns the center of a model (top-left + half tile size).
     */
    [[nodiscard]] std::pair<float, float> getCenter(const EntityModel* model) const;

    /**
     * @brief Parses the map file and creates all world entities.
     *
     * Spawns walls, collectables, Pac-Man, and ghosts according to
     * the characters defined in the map layout.
     */
    void parseMap();

    /**
     * @brief Assigns a visual type to each wall based on its neighbors.
     *
     * Determines whether each wall is an end, corner, straight segment,
     * T-junction, or cross, using the surrounding wall configuration.
     */
    void setWallsTypes() const;

    /**
     * @brief Requests a change of Pac-Man's movement direction.
     *
     * The direction is applied only when a valid turn becomes possible;
     * Pac-Man continues moving in the current direction otherwise.
     */
    void changePacmanDirection(Direction direction);

    /**
     * @brief Checks for collectables colliding with Pac-Man and collects them.
     *
     * Removes collected items from the world and updates the score.
     */
    void collect();

    /**
     * @brief Snaps Pac-Man's X position to the center of a grid column.
     *
     * Used to align Pac-Man vertically when turning or colliding with walls.
     *
     * @param x Column index to snap to.
     */
    void snapVertically(int x) const;

    /**
     * @brief Snaps Pac-Man's Y position to the center of a grid row.
     *
     * Used to align Pac-Man horizontally when turning or colliding with walls.
     *
     * @param y Row index to snap to.
     */
    void snapHorizontally(int y) const;

    /**
     * @brief Returns whether a tile is impassable for movement.
     *
     * Out-of-bounds tiles are treated as walls so that the world edges
     * behave as solid barriers.
     *
     * @param r Row index of the cell.
     * @param c Column index of the cell.
     * @return true if the cell is outside the map or contains a wall;
     *         false if the cell is free.
     */
    bool isWallCell(int r, int c) const;

    /**
     * @brief Handles turning and wall collision logic for Pac-Man.
     *
     * Applies direction changes only when aligned to the grid and
     * prevents Pac-Man from entering wall tiles using predictive blocking.
     *
     * @param dt Time elapsed since the previous update.
     */
    void handleCollision(float dt) const;

    /**
     * @brief Updates the game world for the current frame.
     *
     * Updates all entities, resolves collisions, applies movement,
     * and handles collectable interactions.
     *
     * @param dt Time elapsed since the previous frame.
     */
    void update(float dt);

    /** @brief Returns the set of movement directions available from a cell for normal entities.
     *
     * A direction is available if the neighboring tile is not a wall (out-of-bounds
     * counts as a wall). This does NOT apply ghost-house gate rules; use
     * getAvailableGhostDirectionsAt() for ghosts.
     *
     * @param row Current row.
     * @param col Current column.
     * @return Set of allowed directions from {Up, Down, Left, Right}.
     */
    std::set<Direction> getAvailableDirectionsAt(int row, int col) const;

    /**
     * @brief Removes a collectable from the world and updates the score.
     *
     * @param collectable The collectable that was eaten by Pac-Man.
     */
    void eat(const std::shared_ptr<Collectable>& collectable);

    /**
     * @brief Checks whether two axis-aligned rectangles (AABBs) overlap.
     *
     * @return true if the rectangles intersect; false otherwise.
     */
    bool collides(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) const;

    /**
     * @brief Checks whether the bounding boxes of two models overlap.
     *
     * Uses the models' positions and the tile-sized hitboxes used in this world.
     *
     * @return true if the models intersect; false otherwise.
     */
    bool collides(const EntityModel* m1, const EntityModel* m2) const;

    [[nodiscard]] Score* getScore() const { return _score.get(); }

    /** @brief True if (r,c) is a gate tile. */
    [[nodiscard]] bool isGateCell(int r, int c) const;

    /** @brief True if (r,c) is inside the ghost house (not gate). */
    [[nodiscard]] bool isHouseCell(int r, int c) const;

    int getRows() const { return _rows; }
    int getCols() const { return _cols; }
};
} // namespace logic
