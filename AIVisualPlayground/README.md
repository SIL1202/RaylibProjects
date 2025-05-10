# Graph Visualizer - BFS Simulation with Raylib

This project is a graphical Breadth-First Search (BFS) visualizer implemented in C++ using the [Raylib](https://www.raylib.com/) graphics library. It allows users to observe how BFS explores a graph, with visual cues and step-by-step interaction.

## Features

- Interactive step-by-step BFS traversal
- Node queue visualization (FIFO)
- Directed graph rendering with arrows
- Dynamic node label assignment (`A`, `B`, `C`...)
- Reset functionality to restart the traversal
- Randomized vertex generation for dynamic graphs
- Randomized start point selection for BFS

## Project Structure

```bash
.
├── src/
│   ├── main.cpp         # Entry point & render loop
│   ├── Global.h         # UI and graph declarations
│   ├── Global.cpp       # UI and graph implementation
│   └── Makefile         # Build script
├── assets/
│   ├── triangle.png     # Icon for the "Next" button
│   └── BFS_demo.gif         # Optional demo animation
├── include/
│   ├── raylib.h
│   └── raymath.h
└── lib/
    └── libraylib.a
```

How to Build & Run

### Prerequisites

- C++ compiler (GCC/Clang)
- Raylib (installed and linked)
- Make

### Build

```bash
make
```

## Controls

- Click the triangle button to perform one BFS step
- Click the square reset button to restart
- The queue will update as BFS proceeds

## Demo

![BFS Visual Demo](./assets/BFS_demo.gif)

## Dependencies

- [Raylib](https://github.com/raysan5/raylib) – Simple and easy-to-use graphics library for C++

## License

This project is open source and free to use for learning and educational purposes.