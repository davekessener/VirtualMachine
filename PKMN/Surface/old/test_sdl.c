#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]){

  int i;

  // Declare display mode structure to be filled in.
  SDL_DisplayMode current;

  SDL_Init(SDL_INIT_VIDEO); 

  // Get current display mode of all displays.
  for(i = 0; i < SDL_GetNumVideoDisplays(); ++i){
  
    int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

    if(should_be_zero != 0)
      // In case of error...
      printf("Could not get display mode for video display #%d: %s", i, SDL_GetError());
    
    else 
      // On success, print the current display mode.
      printf("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);

  }
 
  // Clean up and exit the program.
  SDL_Quit();     
  return 0;   
 
}


