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
  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) perror("Failed: ");
  
  char buffer[MAX_LEN]; // Max line length

  while (fgets(buffer, MAX_LEN, fp)){
    // Check if line starts with "v " (vector) or "f " (face)
    int is_vector_line = strncmp(buffer, "v ", 2);
    int is_face_line = strncmp(buffer, "f ", 2);

  if(is_vector_line == 0){
    char* saveptr;  // For maintaining context in strtok_r
    char* token = strtok_r(buffer, " ", &saveptr);
    if (token == NULL) perror("Failed: ");

    // Parse the vertex line; atof converts string to float
    vec3_t vertex;
    vertex.x = atof(strtok_r(NULL, " ", &saveptr));
    vertex.y = atof(strtok_r(NULL, " ", &saveptr));
    vertex.z = atof(strtok_r(NULL, " ", &saveptr));
        
    array_push(mesh.vertices, vertex);
    // printf("Vertex: (%f, %f, %f)\n", vertex.x, vertex.y, vertex.z);
}



  if(is_face_line == 0) { 
    char* saveptr_1;  // For maintaining context in strtok_r for buffer
    char* saveptr_2;  // For maintaining context in strtok_r for face meta data
    char* token = strtok_r(buffer, " ", &saveptr_1);

    if (token == NULL) perror("Failed: ");

    // Parse the face line
    char* face_1_meta_data = strtok_r(NULL, " ", &saveptr_1);
    char* face_2_meta_data = strtok_r(NULL, " ", &saveptr_1);
    char* face_3_meta_data = strtok_r(NULL, " ", &saveptr_1);

    if (!face_1_meta_data || !face_2_meta_data || !face_3_meta_data) {
        perror("Failed to parse face meta data");
    }

    char* face_1 = strtok_r(face_1_meta_data, "/", &saveptr_2);
    char* face_2 = strtok_r(face_2_meta_data, "/", &saveptr_2);
    char* face_3 = strtok_r(face_3_meta_data, "/", &saveptr_2);

    if (!face_1 || !face_2 || !face_3) {
        perror("Failed to parse face number data");
    }

    face_t face = {
        .a = atoi(face_1) - 1, // -1 because obj files start counting from 1
        .b = atoi(face_2) - 1,
        .c = atoi(face_3) - 1
    };

    array_push(mesh.faces, face);
    // printf("Face: (%d, %d, %d)\n", face.a, face.b, face.c);
}

  }
  fclose(fp);
}