#include "../headers/peripherals.h"

using namespace std;
// using rgb_matrix::GPIO;
// using rgb_matrix::RGBMatrix;
// using rgb_matrix::Canvas;

Peripherals::Peripherals(int mode): FPS(60.0){
	
  this->gfx = new short *[32];
  for(int i = 0; i < 32; i++){
      this->gfx[i] = new short[64];
      for(int j = 0; j < 64; j++){
        this->gfx[i][j] = 0;
      }
  }
	this->event_queue = al_create_event_queue();
	if (!event_queue) {
		cerr << "Failed to create event queue." << endl;
	}
	if(mode == 0){
		this->display = al_create_display(1280, 640);

		if (!display) {
			cerr << "Failed to create display." << endl;
		}
		
		al_register_event_source(event_queue, al_get_display_event_source(display));

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_flip_display();

		//this -> canvas = NULL;
	}else{
		// RGBMatrix::Options defaults;

		// defaults.hardware_mapping = "adafruit-hat";
		// defaults.rows = 32;
		// defaults.cols = 64;
		// defaults.chain_length = 1;
		// defaults.parallel = 1;
		// defaults.brightness = 40;

		// Canvas *canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);

		this -> display = NULL;
	}

	al_init_timeout(&timeout, 0.06);
}

Peripherals::~Peripherals(){
	al_destroy_display(this->display);
	al_destroy_event_queue(this->event_queue);
  delete [] gfx;
}

void Peripherals::updateDisplay(){

	al_clear_to_color(al_map_rgb(0, 0, 0));

	for(int i = 0; i < 64; i++){
		for(int j = 0; j < 32; j++){
			if(this->gfx[j][i] == 1){
				al_draw_filled_rectangle(i*20,j*20,i*20+20,j*20+20,al_map_rgb(255,255,255));
			}
		}
	}
	al_flip_display();

	this->toUpdate = false;
}

// void Peripherals::updateLEDMatrix(Canvas* canvas){	
// 	short white[3] = {255,255,255};

// 	for(int i = 0; i < 64; i++){
// 		for(int j = 0; j < 32; j++){
// 			if(this -> gfx[j][i] == 1){
// 				canvas -> setPixel(i,j,white[0],white[1],white[2]);
// 			}
// 		}
// 	}
// 	this->toUpdate = false;
// }