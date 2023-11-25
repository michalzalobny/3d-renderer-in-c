#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

u_int32_t* color_buffer = NULL; // u_int32_t to make sure that each element is exactly 32 bit (4 bytes) long
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Create SDL window
  window = SDL_CreateWindow(
    NULL, // Window title
    SDL_WINDOWPOS_CENTERED, // x position
    SDL_WINDOWPOS_CENTERED, // y position 
    window_width, 
    window_height, 
    SDL_WINDOW_BORDERLESS
  );

  if(!window){
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  //Create an SDL renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  
  if(!renderer){
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  return true;
}

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

void render_color_buffer(){
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int) (window_width * sizeof(uint32_t))
  );

  SDL_RenderCopy(
    renderer,
    color_buffer_texture,
    NULL,
    NULL
  );
}

void clear_color_buffer(uint32_t color){
  for(int y = 0; y < window_height; y++){
    for(int x = 0; x < window_width; x++){
      color_buffer[window_width * y + x] = color;
    }
  }
}

void render(void){
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);

  render_color_buffer();

  clear_color_buffer(0xFFFFFF00);

  SDL_RenderPresent(renderer);
}

void destroy_window(void){
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
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