#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  private:
    unsigned char *gfx;

  public:
    Peripherals();
    ~Peripherals();
    void updateDisplay();
    void generateSFX();
    void getInput();
};

#endif