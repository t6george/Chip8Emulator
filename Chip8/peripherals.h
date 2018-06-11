#include <allegro5/allegro.h>
#include <iostream>

using namespace std;

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  private:
    short**gfx;

  public:
    Peripherals();
    ~Peripherals();
    void updateDisplay();
    void generateSFX();
    void getInput();
};

#endif