#include "mesh.h"
#include "array.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>



mesh_t mesh = {
  .vertices = NULL,
  .faces = NULL,
  .rotation = {.x = 0, .y = 0, .z = 0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
  { .x = -1, .y = -1, .z = -1 }, // 1
  { .x = -1, .y =  1, .z = -1 }, // 2
  { .x =  1, .y =  1, .z = -1 }, // 3
  { .x =  1, .y = -1, .z = -1 }, // 4
  { .x =  1, .y =  1, .z =  1 }, // 5
  { .x =  1, .y = -1, .z =  1 }, // 6
  { .x = -1, .y =  1, .z =  1 }, // 7
  { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
  // front
  { .a = 1, .b = 2, .c = 3 },
  { .a = 1, .b = 3, .c = 4 },
  // right
  { .a = 4, .b = 3, .c = 5 },
  { .a = 4, .b = 5, .c = 6 },
  // back
  { .a = 6, .b = 5, .c = 7 },
  { .a = 6, .b = 7, .c = 8 },
  // left
  { .a = 8, .b = 7, .c = 2 },
  { .a = 8, .b = 2, .c = 1 },
  // top
  { .a = 2, .b = 7, .c = 5 },
  { .a = 2, .b = 5, .c = 3 },
  // bottom
  { .a = 6, .b = 8, .c = 1 },
  { .a = 6, .b = 1, .c = 4 }
};


void load_cube_mesh_data(void){
  for (int i = 0; i < N_CUBE_VERTICES; i++){
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }

  for (int i = 0; i < N_CUBE_FACES; i++){
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
}

#define MAX_LEN 256

void load_obj_file_data(char* filename){
  // Read the contents of the .obj file
  // and load the vertices and faces in 
  // out mesh.vertices and mesh.faces

  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("Failed: ");
  }

  char buffer[MAX_LEN];
  while (fgets(buffer, MAX_LEN, fp)){
    // char* tokens = strtok(buffer, " ");
    bool is_vector_line = strncmp(buffer, "v ", 2) == 0;
    bool is_face_line = strncmp(buffer, "f ", 2) == 0;



    if(is_vector_line){
      print_string(buffer);
    }

  }
  fclose(fp);
 
}