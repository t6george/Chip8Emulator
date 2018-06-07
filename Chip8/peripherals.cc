#include "peripherals.h"
#include <iostream>

//to compile: g++ peripherals.cc -F/Library/Frameworks/SDL2.framework/Headers -framework SDL2

Peripherals::Peripherals(){
  this->gfx = new short *[32];

  for(int i = 0; i < 32; i++){
      this->gfx[i] = new short[64];
      for(int j = 0; j < 64; j++){
        this->gfx[i][j] = 0;
      }
  }
  this->gfx[2][3] = 1;
}

Peripherals::~Peripherals(){
  delete [] gfx;
}

void Peripherals::updateDisplay(){
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow("Chip8",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,320,SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

  //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  //SDL_RenderClear(renderer);
  //SDL_RenderPresent(renderer);

  // r.x = 0;
  // r.y = 0;
  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  // SDL_RenderFillRect(renderer, &r);
  // SDL_RenderPresent(renderer);
  SDL_Rect r;
  r.w = 10;
  r.h = 10;
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 

  for(int i = 0; i < 32; i++){
    for(int j = 0; j < 64; j++){
      if(this->gfx[i][j] == 0){ //change back to 1
        r.x = j*10;
        r.y = i*10;
        SDL_RenderFillRect(renderer, &r);
        SDL_RenderPresent(renderer);
        //SDL_Delay(10);
      }
    }
  }
  
  SDL_Delay(10000);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char*argv[]){
  Peripherals* peripheral = new Peripherals();
  peripheral->updateDisplay();
  return 0;
}

