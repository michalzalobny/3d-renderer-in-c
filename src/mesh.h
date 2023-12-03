#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"

#define N_MESH_VERTICES 8
extern vec3_t mesh_vertices[N_MESH_VERTICES]; // Defined externally in ("in mesh.c")

#define N_MESH_FACES (6 * 2) // 6 faces with 2 triangles each
extern face_t mesh_faces[N_MESH_FACES];

#endif