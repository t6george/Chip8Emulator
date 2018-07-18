#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
// #include "led-matrix.h"
// #include <signal.h>
// #include "graphics.h"

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  public:
    const float FPS;
    bool toUpdate;

    ALLEGRO_TIMEOUT timeout;
		ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE *keyState;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_DISPLAY *display;
    short**gfx;
    
    Peripherals();
    ~Peripherals();

    void updateDisplay();
    //void updateLEDMatrix(Canvas* canvas);
    void generateSFX();
};

#endif