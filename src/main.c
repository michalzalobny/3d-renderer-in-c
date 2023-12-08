#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "helpers.h"

// Array of triangles that should be rendere frame by frame
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};

float fov_factor = 640;

bool is_running = false;

int previous_frame_time = 0;

void setup(void){
  // Allocate the required memory in bytes to hold the color buffer
  color_buffer = (u_int32_t*) malloc(sizeof(u_int32_t) *  window_width * window_height);

  // Creating an SDL texture that is used to display the color_buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );

  // Load the cube values in the mesh data structure
  // load_cube_mesh_data();
  load_obj_file_data("./assets/f22.obj");
}

void process_input(void){
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        is_running = false;
        break;
  }
}

// Convert 3D point to 2D
vec2_t project(vec3_t point){

  float w = 1;

  float x_2 = point.x / point.z * w;

  float rb = sqrt(pow(x_2, 2.0) + pow(w, 2.0));
  float cb = (rb * point.z) / w;

  float y_2 = (point.y * rb) / cb;

  vec2_t projected_point = {
    .x = fov_factor * (x_2),
    .y = fov_factor * (y_2)
  };

  // Old solution (simplified one, works if w = 1)
  // vec2_t projected_point = {
  //   .x = fov_factor * (point.x) / point.z,
  //   .y = fov_factor * (point.y) / point.z
  // };

  return projected_point;
}

void update(void) {
  //Locking the execution if not enough time passed
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if(time_to_wait>0 && time_to_wait <= FRAME_TARGET_TIME){
    SDL_Delay(time_to_wait);
  }
  previous_frame_time = SDL_GetTicks();

  // Clear the array of triangles to render each frame
  array_free(triangles_to_render);
  triangles_to_render = NULL;


  // Rotate the cube
  mesh.rotation.x += 0.01;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.01;

  // Loop all triangle faces of our cube mesh
  int num_faces = array_length(mesh.faces);
  for(int i = 0; i < num_faces; i++){
    face_t mesh_face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a - 1]; // Compensate for the index offset in mesh_vertices arr (they start at 0 , but we start from 1)
    face_vertices[1] = mesh.vertices[mesh_face.b - 1]; 
    face_vertices[2] = mesh.vertices[mesh_face.c - 1]; 

    triangle_t projected_triangle;

    // Loop all three vertices of this current face and apply transformations
    for (int j = 0; j < 3; j++){
      vec3_t transformed_vertex = face_vertices[j];

      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

      // Translate the vertex away from camera in z
      transformed_vertex.z += camera_position.z;

      // Project the current vertex
      vec2_t projected_point = project(transformed_vertex);

      //Scale and translate the projected points to the middle of the screen
      projected_point.x += window_width / 2,
      projected_point.y += window_height / 2,

      projected_triangle.points[j] = projected_point;
    }

    // Save the projected triangle in the array of triangles to render
    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void){
  // draw_grid(0xFF0000FF, 15);

  // Loop all projected triangles and render them
  int num_triangles = array_length(triangles_to_render);

  for (int i = 0; i< num_triangles; i++){
    triangle_t triangle = triangles_to_render[i];

    //Draw vertex points
    draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
    draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
    draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);

    // Draw unfilled triangle
    draw_triangle(
      triangle.points[0].x, triangle.points[0].y,
      triangle.points[1].x, triangle.points[1].y,
      triangle.points[2].x, triangle.points[2].y,
      0xFF00FF00
    );
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  SDL_RenderPresent(renderer);
}

// Free memory that was dynamically allocated by the program
void free_resources(void){
  free(color_buffer);
  array_free(mesh.faces);
  array_free(mesh.vertices);
  array_free(triangles_to_render);
}

int main (void) {
  is_running = initialize_window();


  setup();

 

  while (is_running){
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();

  return 0;
}