#include "../headers/peripherals.h"
 
Peripherals::Peripherals(): FPS(60.0){
	
  this->gfx = new short *[32];
  for(int i = 0; i < 32; i++){
      this->gfx[i] = new short[64];
      for(int j = 0; j < 64; j++){
        this->gfx[i][j] = 0;
      }
  }
	// this->timer = al_create_timer(1.0 / FPS);

	// if (!timer) {
	// 	cerr << "Failed to create timer." << endl;
	// }

	this->display = al_create_display(1280, 640);

	if (!display) {
		cerr << "Failed to create display." << endl;
	}
	
	this->event_queue = al_create_event_queue();

	if (!event_queue) {
		cerr << "Failed to create event queue." << endl;
	}
	
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	al_init_timeout(&timeout, 0.06);
	//al_start_timer(timer);

	this->running = true;
}

Peripherals::~Peripherals(){
	al_destroy_display(this->display);
	al_destroy_event_queue(this->event_queue);
  delete [] gfx;
}

void Peripherals::updateDisplay(){

	al_clear_to_color(al_map_rgb(0, 0, 0));

	if(this->toUpdate){
		for(int i = 0; i < 64; i++){
			for(int j = 0; j < 32; j++){
				if(this->gfx[j][i] == 1){
					al_draw_filled_rectangle(i*20,j*20,i*20+20,j*20+20,al_map_rgb(255,255,255));
				}
			}
		}
		al_flip_display();
	}
	this->toUpdate = false;
}