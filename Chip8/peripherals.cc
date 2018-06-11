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

const float FPS = 60.0;

void Peripherals::updateDisplay(){
  ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool running = true;
	bool redraw = true;

	if (!al_init()) {
		cerr << "Failed to initialize allegro." << endl;
		return;
	}
	
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		cerr << "Failed to create timer." << endl;
		return;
	}

	display = al_create_display(640, 480);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		cerr << "Failed to create event queue." << endl;
		return;
	}

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
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
					break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_flip_display();
			redraw = false;
		}
	} 

	al_destroy_display(display);
	al_destroy_event_queue(event_queue);




  // for(int i = 0; i < 64; i++){
  //   for(int j = 0; j < 32; j++){
  //     if(this->gfx[j][i] == 0){ //change back to 1
  //       int a = j;
  //     }
  //   }
  // }
}

int main(int argc, char*argv[]){
  Peripherals* peripheral = new Peripherals();
  peripheral->updateDisplay();
  return 0;
}

