#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "display.h"

bool is_running = false;

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

void update(void){}

void render(void){
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);

  // draw_grid(0xFFFF0000, 30);
  // draw_rect(100, 200, 400, 90, 0xFF0000FF);
  draw_pixel(100, 200, 0xFFFF0000);

  render_color_buffer();

  clear_color_buffer(0xFFFFFF00);

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