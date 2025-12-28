
# Pac-Man (SFML / C++20) — Advanced Programming Project

A Pac-Man inspired game implemented in modern C++ (C++20) with SFML for graphics/audio.  
The codebase is split into a **logic** library (game rules, no SFML dependency) and an **app/representation** layer (SFML rendering + input), connected through a small factory and observer-style notifications.

---
## Author

Name:           Abdellah El Moussaoui  
Student Number: S20246031

## Features (what’s implemented)

### Core gameplay
- Maze loaded from a text map (`assets/maps/map.txt`)
- Smooth (continuous) movement with wall collisions
- Collectables:
  - **Coins** (`*`) — score depends on time since last coin
  - **Fruits** (`$`) — triggers frightened mode for ghosts
- Lives system: start with **3 lives**
- Score system:
  - Score decreases over time
  - Coins increase score (more if collected quickly)
  - Extra points for fruit / eating ghosts / finishing a level
- Level progression:
  - When all coins are collected, a new level starts
  - Difficulty scales per level (ghost speed up, frightened duration down)

### Ghosts (4 total)
- **1 direct chase ghost**: targets Pac-Man’s current position
- **2 lookahead chase ghosts**: target a point in front of Pac-Man (direction-based prediction)
- **1 “locked” ghost**:
  - stays in a more constrained mode
  - relocks with probability **0.5** at intersections

### Ghost release + frightened mode
- Release timing:
  - **2 ghosts** chase from the start
  - the other **2** start in the center and switch to chase after **5s** and **10s**
- Frightened mode (after eating fruit):
  - ghosts reverse direction on activation
  - ghosts slow down
  - at intersections they choose moves that maximize Manhattan distance from Pac-Man
  - Pac-Man can eat frightened ghosts → ghost respawns in the center

---

## Controls
- Move: **WASD** and/or **Arrow keys** (depending on build/settings)
- Start game from menu: **Enter**
- Close window: standard window close / Escape (if enabled)

---

## Map legend (`assets/maps/map.txt`)
- `#` wall
- `*` coin
- `$` fruit
- `@` ghost spawn / center area
- `P` Pac-Man spawn
- `&` gate

Fruits are placed in the **upper-left** and **bottom-right** of the maze.

---

## Project structure
```
pac-man/
├─ logic/                 # Game rules, entities, world simulation (no SFML)
│  ├─ include/
│  └─ src/
├─ app/                   # SFML application + rendering layer
│  ├─ include/
│  └─ src/
│  └─ main.cpp
├─ assets/                # Sprites, audio, fonts, map(s)
└─ CMakeLists.txt
```

### Architecture overview (high level)
- **State system** (menu / level / overlays) driven by a `StateManager`
- **AbstractFactory** bridges logic ↔ representation creation (models + views)
- **Observer/Subject** style notifications used for view updates
- **Camera** maps world coordinates to screen coordinates and handles resizing

---

## Build & Run

### Dependencies
- **CMake** (3.16+ recommended)
- **C++20** compiler (GCC/Clang/MSVC)
- **SFML 2.6+** (graphics, window, system, audio)

### Build (generic CMake)
From the project root:
```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Run the executable from the build output (name depends on your target setup):
```bash
./build/app/pacman_app
```
