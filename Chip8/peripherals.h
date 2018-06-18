#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

using namespace std;

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

class Peripherals{
  public:
    const float FPS;
    ALLEGRO_TIMER *timer;
    ALLEGRO_TIMEOUT timeout;
		ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_DISPLAY *display;
    bool get_event;

    bool running;
    bool toUpdate;
    short**gfx;

    Peripherals();
    ~Peripherals();
    void updateDisplay();
    void generateSFX();
    void getInput();
};

#endif