#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  public:
    const float FPS;
    bool toUpdate;

    //ALLEGRO_TIMER *timer;
    ALLEGRO_TIMEOUT timeout;
		ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE *keyState;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_DISPLAY *display;
    bool running;
    short**gfx;
    
    Peripherals();
    ~Peripherals();
    void updateSignal(bool sig);
    void updateDisplay();
    void generateSFX();
};

#endif