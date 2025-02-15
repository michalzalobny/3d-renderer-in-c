# 3D Renderer in C

Building a 3D graphics renderer from scratch using the C programming language. Based on [Gustavo Pezzi's course](https://pikuma.com/courses/learn-3d-computer-graphics-programming)

No GPU, or OpenGL. Just a C compiler and linear algebra.

In this project, I'm using the Left-Handed coordinate system (Z-values increase when going into the screen). I also use column-major matrices layout.

Check out also the [WebGL 3D engine](https://github.com/michalzalobny/webgl-viewer), that I created after finishing this project.

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

# Run commands

- **Using custom script**

  - `./build-and-run.sh`

- **Using Makefile**

  - `make build` or just `make` (it will pick first command)
  - `make run`
  - `make clean`

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

# Importing mesh with Blender

When importing your own mesh, make sure to export it as a Wavefront (.obj) file. Then, in the export settings, make sure to check the following options:

- Apply Modifiers
- Normals
- UV Coordinates
- Triangulate Faces (so that the mesh is made of triangles, not quads)

# Images from the project

![1](docs/1.png)
![2](docs/2.png)
![3](docs/3.png)
![4](docs/4.png)
![5](docs/5.png)
![6](docs/6.png)
