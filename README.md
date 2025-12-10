# Pac-Man — Advanced Programming Project

This repository contains my implementation of the Pac-Man game for the course **Gevorderd Programmeren** at the University of Antwerp.  
The project follows the official specification and focuses on modular architecture, clear separation of concerns, correct use of design patterns, and maintainable C++ code.

## UML Diagram

[View Full UML Diagram](https://miro.com/app/live-embed/uXjVGdQWha8=/?embedMode=view_only_without_ui&moveToViewport=-2014%2C-210%2C5044%2C2679&embedId=27473108072)

## Project Structure

```
pacman/
│
├── logic/                  
│   ├── include/             # Pure game logic (NO SFML)
│   ├── src/
│   └── CMakeLists.txt
│
├── app/                    
│   ├── include/             # SFML representation layer
│   ├── src/
│   └── CMakeLists.txt
│
├── assets/                  # Sprites, fonts, maps
│
├── CMakeLists.txt           # Root build file
└── README.md
```

## Features

### Logic Module (`libpaclogic`)
- Tile-based world model  
- Pac-Man movement and collision handling  
- Coin and fruit collection  
- Score system with time-based modifiers  
- Four ghosts with three AI behaviors:
  - Locked movement ghost (50% relock at intersections)
  - Two predictive ghosts (chase the tile in front of Pac-Man)
  - Direct chaser ghost (minimizes Manhattan distance)
- Observer pattern for event-driven updates
- Stopwatch singleton for deltaTime computation  
- Random singleton (Mersenne Twister) for AI randomness

### App Module (SFML)
- Rendering of maze, Pac-Man, ghosts, and items  
- Input handling  
- Frame-rate-independent movement  
- Game states:
  - Menu  
  - Level  
  - Paused  
  - Victory  

## Design Patterns

### Observer Pattern
Used between logic models (Subjects) and observers such as Score and entity views.

### Singleton Pattern
- `Stopwatch` — deltaTime timing using `std::chrono::steady_clock`
- `Random` — random number generation for ghost AI

### State Pattern
Used to manage:
- MenuState  
- LevelState  
- PausedState  
- VictoryState  

## Build Instructions

### Requirements
- C++20  
- CMake ≥ 3.16  
- SFML ≥ 2.6.1  

Install SFML on Ubuntu:

```bash
sudo apt install libsfml-dev
```

### Build

```bash
cmake -B build
cmake --build build
```

### Run

```bash
./build/app/pacman_app
```

## Code Quality Notes

- `.clang-format` ensures consistent formatting  
- Logic and representation fully separated  
- No global variables  
- No SFML code inside the logic module  
- No logic code inside the app module  
- DeltaTime used everywhere (no busy waiting)

## Author

Your Name  
Advanced Programming — University of Antwerp
