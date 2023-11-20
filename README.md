# 3D Renderer in C

Building a 3D graphics renderer from scratch using the C programming language.

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

Based on Gustavo Pezzi's course

# Run commands

- **Compiling the program**

  - `gcc main.c -o output`

- **Running the program**
  - `./output`

# Linux Installation

- **Build Tools**
  - **Command:** `sudo apt install build-essential`
- **SDL2**
  - **Command:** `sudo apt install libsd12-dev`

# Mac Installation

- **Xcode Command Line Tools**
  - **Command**: `xcode-select --install`.
- **SDL2 Library**
  - **Command via Homebrew**: If Homebrew is not installed, first install it from [brew.sh](https://brew.sh/). Then, `brew install sdl2` in the Terminal.
