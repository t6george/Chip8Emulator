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

  for(int i = 0; i < 64; i++){
    for(int j = 0; j < 32; j++){
      if(this->gfx[j][i] == 0){ //change back to 1
        //r.x = i*10;
        //r.y = j*10;
        int a = j;
      }
    }
  }
}

int main(int argc, char*argv[]){
  Peripherals* peripheral = new Peripherals();
  peripheral->updateDisplay();
  return 0;
}

