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
  // SDL_Init(SDL_INIT_EVERYTHING);
  // SDL_Window *window = SDL_CreateWindow("Chip8",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,320,SDL_WINDOW_SHOWN);
  

  // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  // SDL_RenderClear(renderer);

  // r.x = 0;
  // r.y = 0;
  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  // SDL_RenderFillRect(renderer, &r);
  // SDL_RenderPresent(renderer);
  // SDL_Rect r;
  // r.w = 10;
  // r.h = 10;
  // r.x = 10;
  // r.y = 10;
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
  // SDL_RenderPresent(renderer);
  // SDL_Delay(5000);
  // r.x = 20;
  // r.y = 20;
  // SDL_RenderPresent(renderer);

  // for(int i = 0; i < 64; i++){
  //   for(int j = 0; j < 32; j++){
  //     if(this->gfx[j][i] == 0){ //change back to 1
  //       r.x = i*10;
  //       r.y = j*10;
  //       SDL_RenderFillRect(renderer, &r);
  //       //SDL_Delay(1000);
  //       SDL_RenderPresent(renderer);
  //       SDL_Delay(1000);
  //     }
  //   }
  // }
      SDL_Window* window = NULL;
      SDL_Surface* surface = NULL;
      //SDL_Rect rects[15][13];
      if (SDL_Init(SDL_INIT_VIDEO) < 0){
          printf("SDL_Error: %s\n", SDL_GetError());
      }else{
        window = SDL_CreateWindow("SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN); //Creates the window
        if (window == NULL){
            printf("SDL_Error: %s\n", SDL_GetError());
        }else{
            SDL_Renderer* renderer = NULL;
            renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED); //renderer used to color rects

            SDL_SetRenderDrawColor(renderer, 51, 102, 153, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < 14; i++){
                for (int j = 0; j < 12; j++){
                  SDL_Rect rectangular;
                  rectangular.x = i*10;
                  rectangular.y = j*10;
                  rectangular.w = 10;
                  rectangular.h = 10;
                  //rects[i][j] = rectangular;
                  SDL_SetRenderDrawColor(renderer, 255, 102, 0, 255);
                  SDL_RenderFillRect(renderer, &rectangular);
                }
            }
            SDL_UpdateWindowSurface(window);
            SDL_Delay(5000);
        }
    }

  SDL_Delay(5000);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char*argv[]){
  Peripherals* peripheral = new Peripherals();
  peripheral->updateDisplay();
  return 0;
}

