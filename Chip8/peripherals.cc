#include "peripherals.h"

Peripherals::Peripherals(){
  gfx = new unsigned char[64 * 32]; //Monochrome display bits
}

Peripherals::~Peripherals(){
  delete [] gfx;
}

