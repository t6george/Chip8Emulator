#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  private:
    short**gfx;
    const float FPS;
    bool running;
    bool redraw;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;

  public:
    Peripherals();
    ~Peripherals();
    void updateDisplay();
    void generateSFX();
    void getInput();
};

#endif