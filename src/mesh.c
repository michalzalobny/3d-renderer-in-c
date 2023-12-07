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
  //Free the mesh vertices and faces if they are not NULL
  if(mesh.vertices != NULL) free(mesh.vertices);
  if(mesh.faces != NULL) free(mesh.faces);

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

  if(mesh.vertices != NULL) free(mesh.vertices);
  if(mesh.faces != NULL) free(mesh.faces);

  FILE* fp;
  fp = fopen(filename, "r");
  if (fp == NULL) perror("Failed: ");
  
  char buffer[MAX_LEN]; // Max line length

  while (fgets(buffer, MAX_LEN, fp)){
    // Find line that starts with "v " (vector) or "f " (face)
    int is_vector_line = strncmp(buffer, "v ", 2);
    int is_face_line = strncmp(buffer, "f ", 2);

    if(is_vector_line == 0){
      char* first_token = strtok(buffer, " ");  // Start tokenizing the line by space

      // Parse the vertex line; atof converts string to float and strtok with NULL continues to next token
      vec3_t vertex = {
        .x = atof(strtok(NULL, " ")),
        .y = atof(strtok(NULL, " ")),
        .z = atof(strtok(NULL, " "))
      };

      // Push the vertex to the model_vertices array
      array_push(mesh.vertices, vertex);
      //Print vertex
      printf("Vertex: (%f, %f, %f)\n", vertex.x, vertex.y, vertex.z);
    }

    if(is_face_line == 0) { 
      char* first_token = strtok(buffer, " ");  // Start tokenizing the line by space
      // Parse the face line
      char* face_1_meta_data = strtok(NULL, " ");
      char* face_2_meta_data = strtok(NULL, " ");
      char* face_3_meta_data = strtok(NULL, " ");

      char* face_1 = strtok(face_1_meta_data, "/");
      char* face_2 = strtok(face_2_meta_data, "/");
      char* face_3 = strtok(face_3_meta_data, "/");

      face_t face = {
        .a = atoi(face_1) - 1, // -1 because obj files start counting from 1
        .b = atoi(face_2) - 1,
        .c = atoi(face_3) - 1
      };

      // Push the face to the mesh.faces array
      array_push(mesh.faces, face);
      // Print face
      printf("Face: (%d, %d, %d)\n", face.a, face.b, face.c);
    }

 
  }
  
  fclose(fp);
}