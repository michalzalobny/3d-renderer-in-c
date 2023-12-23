#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

// Array of triangles that should be rendere frame by frame
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

float fov_factor = 640;

bool is_running = false;

bool CULL_BACKFACE = true;
bool RENDER_WIREFRAME = true;
bool RENDER_FILL = true;
bool RENDER_VERTICES = true;

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
  load_obj_file_data("./assets/cube.obj");
}

void handle_key_press(SDL_Keycode keycode){
  switch(keycode){
    case SDLK_ESCAPE:
      is_running = false;
      break;
    case SDLK_1:
      CULL_BACKFACE = !CULL_BACKFACE;
      break;
    case SDLK_2:
      RENDER_WIREFRAME = !RENDER_WIREFRAME;
      break;
    case SDLK_3:
      RENDER_FILL = !RENDER_FILL;
      break;
    case SDLK_4:
      RENDER_VERTICES = !RENDER_VERTICES;
      break;
  }
}

void process_input(void){
  SDL_Event event;
  SDL_PollEvent(&event);

  switch(event.type){
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      handle_key_press(event.key.keysym.sym);
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

    // Loop all triangle faces of our mesh
    int num_faces = array_length(mesh.faces);
    for (int i = 0; i < num_faces; i++) {
        face_t mesh_face = mesh.faces[i];

        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[mesh_face.a - 1];
        face_vertices[1] = mesh.vertices[mesh_face.b - 1];
        face_vertices[2] = mesh.vertices[mesh_face.c - 1];

        vec3_t transformed_vertices[3];

        // Loop all three vertices of this current face and apply transformations
        for (int j = 0; j < 3; j++) {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate the vertices away from the camera
            transformed_vertex.z += 5;

            transformed_vertices[j] = transformed_vertex;
        }

        if(CULL_BACKFACE){
          // Check backface culling
          vec3_t vector_a = transformed_vertices[0]; /*   A   */
          vec3_t vector_b = transformed_vertices[1]; /*  / \  */
          vec3_t vector_c = transformed_vertices[2]; /* C---B */

          // Get the vector subtraction of B-A and C-A
          vec3_t vector_ab = vec3_sub(vector_b, vector_a);
          vec3_t vector_ac = vec3_sub(vector_c, vector_a);
          vec3_normalize(&vector_ab);
          vec3_normalize(&vector_ac);

          // Compute the face normal (using cross product to find perpendicular)
          vec3_t normal = vec3_cross(vector_ab, vector_ac);
          vec3_normalize(&normal);

          vec3_t camera_ray = vec3_sub(camera_position, vector_a);

          // How aligned the camera ray is with the face normal
          float dot_normal_camera = vec3_dot(normal, camera_ray);

          // Don't render if not facing camera
          if (dot_normal_camera < 0) {
              continue;
          }
        }

        triangle_t projected_triangle;

        // Loop all three vertices to perform projection
        for (int j = 0; j < 3; j++) {
            vec2_t projected_point = project(transformed_vertices[j]);

            // Center
            projected_point.x += (window_width / 2);
            projected_point.y += (window_height / 2);

            projected_triangle.points[j] = projected_point;
        }

        array_push(triangles_to_render, projected_triangle);
    }
}

void render(void){
  // draw_grid(0xFF0000FF, 15);

  // Fill background with gray color
  clear_color_buffer(0xFF151515);

  // Loop all projected triangles and render them
  int num_triangles = array_length(triangles_to_render);

  for (int i = 0; i< num_triangles; i++){
    triangle_t triangle = triangles_to_render[i];

    if(RENDER_VERTICES){
      float vw = 8.0; // vertex width
      //Draw vertex points
      draw_rect(triangle.points[0].x - vw / 2, triangle.points[0].y - vw / 2, vw, vw, 0xFFFFFF00);
      draw_rect(triangle.points[1].x - vw / 2, triangle.points[1].y - vw / 2, vw, vw, 0xFFFFFF00);
      draw_rect(triangle.points[2].x - vw / 2, triangle.points[2].y - vw / 2, vw, vw, 0xFFFFFF00);
    }

    if(RENDER_FILL){
      // Draw filled triangle
      draw_filled_triangle(
        triangle.points[0].x, triangle.points[0].y,
        triangle.points[1].x, triangle.points[1].y,
        triangle.points[2].x, triangle.points[2].y,
        0xFF555555
      );
    }
    if(RENDER_WIREFRAME){
      // Draw unfilled triangle
      draw_triangle(
        triangle.points[0].x, triangle.points[0].y,
        triangle.points[1].x, triangle.points[1].y,
        triangle.points[2].x, triangle.points[2].y,
        0xFF000000
      );
    }

   
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