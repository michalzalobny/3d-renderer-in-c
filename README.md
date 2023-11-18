# 3D Renderer in C

Building a 3D graphics renderer from scratch using the C programming language based on Gustavo Pezzi's course.

No GPU, or OpenGL. Just a C compiler and linear algebra.

## Content

- Pure C and SDL
- Foundations of 3D Graphics
- Trigonometry and linear algebra (matrices, vectors, etc.)
- Vertices and meshes
- 3D projection and perspective
- Rasterizing lines and triangles
- Handling textures and texture mapping
- Camera and view transformations
- Displaying complex 3D models (imported from external programs like Blender)

# Linux Installation

- **Build Tools**

  - **Command:** `sudo apt install build-essential`
  - **Description:** Includes compilers like GCC, make, and other essential build tools.

- **Simple DirectMedia Layer 2 (SDL2)**

  - **Command:** `sudo apt install libsd12-dev`
  - **Description:** Development library for SDL2, used for developing cross-platform multimedia software.

# Mac Installation

This section covers the installation process for building the 3D renderer on a Mac, especially for ARM-based Macs like the M1 or M2.

## Prerequisites

- **Xcode Command Line Tools**: Provides compilers and other essential build tools.

  - **Installation Command**: Open Terminal and run `xcode-select --install`.

- **SDL2 Library**: Required for graphics and event handling.
  - **Installation via Homebrew**: If Homebrew is not installed, first install it from [brew.sh](https://brew.sh/). Then, run `brew install sdl2` in the Terminal.

After installing these components, your Mac will be set up for developing the 3D renderer using C and SDL.
