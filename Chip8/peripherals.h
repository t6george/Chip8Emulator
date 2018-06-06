#include "SDL.h"

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  unsigned char *gfx;
  Peripherals();
  ~Peripherals();
  void updateDisplay();
  void generateSFX();
  void getInput();
};

#endif