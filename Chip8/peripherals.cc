#include "peripherals.h"

//to compile: g++ peripherals.cc -F/Library/Frameworks/SDL2.framework/Headers -framework SDL2

Peripherals::Peripherals(){
  //gfx = new unsigned char[64 * 32]; //Monochrome display bits
}

Peripherals::~Peripherals(){
  //delete [] gfx;
}

void Peripherals::updateDisplay(){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Chip8",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,320,SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

  SDL_SetRenderDrawColor(renderer,0,255,0,255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  SDL_Delay(3000);
}

int main(int argc, char*argv[]){
  Peripherals* peripheral = new Peripherals();
  return 0;
}

