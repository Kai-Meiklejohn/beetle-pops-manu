# Beetle Pops a Manu

A small 2D platformer being developed in modern C++ using SFML.

This project is primarily a hands-on way for me to learn C++, game programming,
and good software engineering practices while building something fun and playable.

## About the Game

The player controls Beetle through a light-hearted side-scrolling platforming level
featuring movement, jumping, platforms, hazards, enemies, collectibles, and water.

The main mechanic is performing a **manu**: launching into water to create the
largest and most ridiculous splash possible.

The quality of a manu may eventually depend on factors such as:

- Jump height
- Downward velocity
- Entry angle
- Timing

Large splashes could knock enemies away, activate objects, break obstacles, or
award additional score.

## Project Status

The project is in very early development.

Currently implemented:

- SFML window creation
- Basic CMake build configuration

The first milestone is one complete playable level.

## Planned First Milestone

- Player movement
- Jumping and gravity
- Platform and tile collision
- A simple tile-based level
- Camera following the player
- Basic enemies
- Collectibles
- Water areas
- A basic manu and splash mechanic
- Player death and level restart
- A clear level objective

## Technical Goals

This is a C++ learning project. Development will focus on:

- Modern C++ practices
- Clear ownership and object lifetimes
- RAII
- Appropriate use of references and `const`
- Simple, maintainable classes
- Composition over unnecessary inheritance
- Separation between game logic and rendering
- Sensible use of STL containers
- Avoiding unnecessary copying and allocations
- Keeping the code easy to debug and extend

The development priority is:

> Make it work → understand the C++ → make the code clean → optimise when necessary.

## Technology

- C++17
- SFML 3.1
- CMake 3.28 or newer
- Visual Studio 2022 on Windows

SFML is downloaded automatically when CMake configures the project.

## Building on Windows

### Visual Studio 2022

1. Install the **Desktop development with C++** workload.
2. Clone this repository.
3. Open the repository folder in Visual Studio.
4. Wait for CMake configuration to finish.
5. Select the executable target.
6. Build and run the project.

### Command Line

```powershell
git clone https://github.com/Kai-Meiklejohn/beetle-pops-manu.git
cd beetle-pops-manu
cmake -B build
cmake --build build --config Debug