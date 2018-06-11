#include "peripherals.h"
#include <iostream>

//to compile: g++ peripherals.cc -F/Library/Frameworks/SDL2.framework/Headers -framework SDL2
 
Peripherals::Peripherals(): FPS(60.0){
  this->gfx = new short *[32];

  for(int i = 0; i < 32; i++){
      this->gfx[i] = new short[64];
      for(int j = 0; j < 64; j++){
        this->gfx[i][j] = 0;
      }
  }

	this->timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		cerr << "Failed to create timer." << endl;
	}

	this->display = al_create_display(640, 320);
	if (!display) {
		cerr << "Failed to create display." << endl;
	}
	
	this->event_queue = al_create_event_queue();
	if (!event_queue) {
		cerr << "Failed to create event queue." << endl;
	}

	this->running = true;
	this->redraw = true;
}

Peripherals::~Peripherals(){
  delete [] gfx;
}


void Peripherals::updateDisplay(){
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	al_start_timer(timer);

	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		al_init_timeout(&timeout, 0.06);

		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		if (get_event) {
			switch (event.type) {
				case ALLEGRO_EVENT_TIMER:
					redraw = true;
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					cerr << "Unsupported event received." << endl;
					break;
			}
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		for(int i = 0; i < 64; i++){
			for(int j = 0; j < 32; j++){
				if(this->gfx[j][i] == 1){
					al_draw_filled_rectangle(i*10,j*10,i*10+10,j*10+10,al_map_rgb(255,255,255));
				}
			}
  }
		al_flip_display();
	} 

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}

int main(int argc, char*argv[]){
	if (!al_init()) {
		cerr << "Failed to initialize allegro." << endl;
		return 1;
	}
  Peripherals* peripheral = new Peripherals();
  peripheral->updateDisplay();
  return 0;
}

