#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "display.h"
#include "vector.h"

// Declare an array of vectors/points
#define N_POINTS (9*9*9)
vec3_t cube_points[N_POINTS]; // 9x9x9 cube
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

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

  // Start loading array of vectors for cube
  // From -1 to 1 - in this 9x9x9 cube
  int point_count = 0;
  for (float x = -1; x<=1; x += 0.25){
    for (float y = -1; y<=1; y += 0.25){
      for (float z = -1; z<=1; z += 0.25){
        vec3_t new_point = {.x = x, .y = y, .z = z}; // Assign like in object so, for field x (.x) assign x etc.
        cube_points[point_count++] = new_point; // Apart from setting new_point at point_count pos, also increment it.
      }
    }
  }

 printf("Points created: %d \n", point_count);
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

  // Rotate the cube
  cube_rotation.x += 0.01;
  cube_rotation.y += 0.01;
  cube_rotation.z += 0.01;

  for(int i = 0; i < N_POINTS; i++){
    vec3_t point = cube_points[i];

    vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
    transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
    transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);
   
 
    // Move the point away from the camera
    transformed_point.z -= camera_position.z;

    //Project the current point
    vec2_t projected_point = project(transformed_point);

    //Save projected 2D vec to array of projected points
    projected_points[i] = projected_point;
  }

}

void render(void){
  // draw_grid(0xFF0000FF, 15);

  // Loop all projected points and render them
  for (int i = 0; i< N_POINTS; i++){
    vec2_t projected_point = projected_points[i];
    draw_rect(
      projected_point.x + window_width / 2,
      projected_point.y + window_height / 2,
      5,
      5,
      0xFFFF0000
    );
  }

  render_color_buffer();

  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
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

  return 0;
}