#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
u_int32_t *color_buffer = NULL; 
SDL_Texture *color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // Get window width and height
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  // Makes sure that I use all available pixels in the real full screen mode set later
  window_width = display_mode.w;
  window_height = display_mode.h;

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

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  return true;
}

void draw_grid(uint32_t color, int gap_size){
   for(int y = 0; y < window_height; y++){
    for(int x = 0; x < window_width; x++){
      if (x % gap_size != 0 && y % gap_size != 0) continue;
      color_buffer[window_width * y + x] = color;
    }
  }
}

void draw_rect(int start_x, int start_y, int width, int height, uint32_t color){
  for(int i = 0; i< width; i++){
    for(int j = 0; j< height; j++){
      int current_x = start_x + i;
      int current_y = start_y + j;
      draw_pixel(current_x, current_y, color);
    }
  }
  // Previous code: much much slower (because we go through ALL pixels on screen - not just the one we need):
  //  for(int y = 0; y < window_height; y++){
  //   for(int x = 0; x < window_width; x++){
  //     if(x>= start_x && y>= start_y && x<= (width + start_x) && y<= (height + start_y)){
  //       // color_buffer[window_width * y + x] = color;
  //       draw_pixel(x, y, color);
  //     }
 
  //   }
  // }
}

void draw_pixel(int x, int y, uint32_t color){
  if(x>=0 && x<window_width && y>=0 && y <window_height){
  color_buffer[window_width * y + x] = color;
  };
}

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

void destroy_window(void){
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

