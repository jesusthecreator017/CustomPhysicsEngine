# CustomPhysicsEngine

[![GitHub Issues](https://img.shields.io/github/issues/jesusthecreator017/CustomPhysicsEngine.svg)](https://github.com/jesusthecreator017/CustomPhysicsEngine/issues)
[![GitHub Stars](https://img.shields.io/github/stars/jesusthecreator017/CustomPhysicsEngine.svg)](https://github.com/jesusthecreator017/CustomPhysicsEngine/stargazers)

## Overview

**CustomPhysicsEngine** is a physics simulation engine designed to model and visualize particle interactions in a two-dimensional space. This project serves as both an educational tool for understanding physics simulations and a foundation for more complex physics-based applications.

## Features

- **Particle Initialization**: Randomized positioning, velocity, acceleration, and color assignments.
- **Physics Calculations**: Incorporates gravitational forces and basic motion equations.
- **Cross-Platform Compatibility**: Supports macOS, Linux, and Windows systems.
- **Modular Design**: Clean separation between initialization, update, and rendering processes.

## Getting Started

### Prerequisites

- **C Compiler**: Ensure you have `gcc` installed. For Windows users, [MinGW](http://www.mingw.org/) is recommended.
- **Raylib**: The project utilizes [raylib](https://www.raylib.com/) for rendering. Installation instructions can be found [here](https://github.com/raysan5/raylib#installation).

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/jesusthecreator017/CustomPhysicsEngine.git
   cd CustomPhysicsEngine
   ```

2. **Build the Project**:
   
   - **On macOS/Linux**:
     ```bash
     make
     ```
   
   - **On Windows (Using MinGW)**:
     ```bash
     make winrun
     ```

   *Note*: The provided `Makefile` automatically detects your operating system and applies the appropriate compiler flags.

### Running the Simulation

After building the project:

- **On macOS/Linux**:
  ```bash
  ./bin/main
  ```

- **On Windows**:
  ```bash
  .\bin\main.exe
  ```

## Usage

Upon running the simulation, a window will display particles influenced by gravitational forces. Users can observe how particles interact over time. For customization:

- **Adjust Particle Properties**: Modify the `initializeParticles` function in `src/particle.c` to change initial conditions.
- **Alter Physics Calculations**: Update the `computeForce` function to introduce new forces or modify existing ones.

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-xyz`).
3. Commit your changes (`git commit -am 'Add feature xyz'`).
4. Push to the branch (`git push origin feature-xyz`).
5. Open a Pull Request.

Please ensure your code adheres to the project's coding standards and includes appropriate documentation.


## Acknowledgements

- [raylib](https://www.raylib.com/) for the graphics library.
- Community contributors for feedback and improvements.

