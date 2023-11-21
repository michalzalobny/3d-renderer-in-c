#include <stdio.h>
#include <SDL2/SDL.h>

void initialize_window(void){
  SDL_Init(SDL_INIT_EVERYTHING);
}

int main (void) {
  
  initialize_window();

  return 0;
}