#include "../headers/cpu.h"

using namespace std;

Chip8Cpu::Chip8Cpu(){
    memory = new unsigned short[0x1000]; //4KB of RAM
    V = new unsigned short[16];        //16 8-bit registers
    I = 0x0;
    pc = 0x200;
    delay_timer = 0;  //operates at 60Hz
    sound_timer = 0;

    stack = new unsigned short[16]; //stack of return addresses after subroutines
    sp = 0;

    keyInputs = new unsigned char[16];
    this->resetKeys();
}

void Chip8Cpu::resetKeys(){
  for(int i = 0; i < 16; i++){
    this->keyInputs[i] = 0;
  }
}

void Chip8Cpu::setKey(int ind){
  this->keyInputs[ind] = 1;
}

Chip8Cpu::~Chip8Cpu(){
  delete [] memory;
  delete [] V;
  delete [] stack;
  delete [] keyInputs;
}

bool Chip8Cpu::loadProgram(char* fileName){
  cout << "Loading ROM..." << endl;

  FILE* rom = fopen(fileName, "rb");
  if (nullptr == rom) {
    cerr << "Failed to open ROM" << endl;
    return false;
  }
    // Get file size
  fseek(rom, 0, SEEK_END);
  long rom_size = ftell(rom);
  rewind(rom);

  // Allocate memory to store rom
  char* rom_buffer = new char[rom_size];

  if (rom_buffer == NULL) {
      cerr << "Failed to allocate memory for ROM" << endl;
      return false;
  }

  // Copy ROM into buffer
  size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
  if (result != rom_size) {
      cerr << "Failed to read ROM" << endl;
      return false;
  }

  // Copy buffer to memory
  if ((0x1000-0x200) > rom_size){
      for (int i = 0; i < rom_size; ++i) {
          memory[0x200 + i] = (uint8_t)rom_buffer[i];   // Load into memory starting
                                                      // at 0x200 (=512)
                                                      //cout << hex << memory[0x200 + i] << endl;
      }
  }
  else {
      cerr << "ROM too large!" << endl;
      return false;
  }

  fclose(rom);
  delete[] rom_buffer;

  cout << "Great Success!" << endl;
  return true;

}

void Chip8Cpu::loadFont(){
  cout << "Loading Fonts..." << endl;
  unsigned short fontset[80] = { 
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70, 
    0xf0, 0x10, 0xf0, 0x80, 0xf0, 
    0xf0, 0x10, 0xf0, 0x10, 0xf0, 
    0x90, 0x90, 0xf0, 0x10, 0x10, 
    0xf0, 0x80, 0xf0, 0x10, 0xf0, 
    0xf0, 0x80, 0xf0, 0x90, 0xf0, 
    0xf0, 0x10, 0x20, 0x40, 0x40, 
    0xf0, 0x90, 0xf0, 0x90, 0xf0, 
    0xf0, 0x90, 0xf0, 0x10, 0xf0, 
    0xf0, 0x90, 0xf0, 0x90, 0x90, 
    0xe0, 0x90, 0xe0, 0x90, 0xe0, 
    0xf0, 0x80, 0x80, 0x80, 0xf0, 
    0xe0, 0x90, 0x90, 0x90, 0xe0, 
    0xf0, 0x80, 0xf0, 0x80, 0xf0, 
    0xf0, 0x80, 0xf0, 0x80, 0x80}; 
  for(int i = 0; i < 80; i++){
    memory[0x50 + i] = (unsigned short)((fontset[i]) & 0xFF);
  }
  
}

void Chip8Cpu::run(Peripherals& peripherals){
  unsigned int opcode = (unsigned int)memory[pc] << 8 | (unsigned int)memory[pc+1];
  cout << "opcode: 0x" << hex << opcode << endl;

  switch(opcode & 0xF000){
    case 0x0000:{
      switch(opcode & 0x00FF){
        case 0x00E0:{ //0x00E0: clear the screen 
          for(int i = 0; i < 32; i++){
            for(int j = 0; j < 64; j++){
              peripherals.gfx[i][j] = 0;
            }
          }
          pc += 2;
          break;
        }
        case 0x00EE:{ //return from subroutine
          sp--;
          pc = (unsigned short)(stack[sp]+2);
          break;
        }
        default: //Ox0NNN
          break;
          //rca 1802 program
        }
      break;
    }

    case 0x1000:{ //0x1NNN: jump to NNN
      pc = (unsigned short)(opcode & 0x0FFF);
      break;
    }
    case 0x2000:{ //0x2NNN: call subroutine at address NNN
      stack[sp] = pc; //save where program counter was at on the stack
      sp++; //increment stack
      pc = (unsigned short)(opcode & 0x0FFF); //jump to NNN: address of desired subroutine
      break;
    }

    case 0x3000:{ //0x3XNN: skips next opcode if VX == NN
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if(this -> V[X] == NN){
        pc += 4;
      }else{
        pc += 2;
      }   
      break;
    }
    case 0x4000:{ //0x4XNN: skip the next instruction if VX != NN
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if(this -> V[X] != NN){
        pc += 4;
      }else{
        pc += 2;
      }   
      break;
    }
    case 0x5000:{ //0x5XY0: skip next instruction if VX == VY
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short Y = (opcode & 0x00F0) >> 4;
      if(this -> V[X] == this -> V[Y]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }
    case 0x6000:{ //0x6XNN: set VX to NN
      int X = (opcode & 0x0F00) >> 8;
      V[X] = (unsigned short)(opcode & 0x00FF); //set register X to NN (8 bits)
      pc += 2; 
      break;
    }
    case 0x7000:{ // 0x7XNN: adds NN to VX
      int X = (opcode & 0x0F00) >> 8;
      int NN = opcode & 0x00FF;
      V[X] = (unsigned short)((V[X] + NN) & 0xFF);
      pc += 2;
      break;
    }

    case 0x8000:{
      switch(opcode & 0x000F){
        case 0x0000:{ // 0x8XY0: set VX to VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = V[Y];
          pc += 2;
          break;
        }
        case 0x0001:{ //0x8XY1: set VX to VX OR VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = (unsigned short)(V[X] | V[Y]);
          pc += 2;
          break;
        }
        case 0x0002:{ //0x8XY2: sets VX to VX & VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[Y] = (unsigned short)(V[X] & V[Y]);
          pc += 2;
          break;
        }
        case 0x0003:{ //0x8XY3: set VX to VX XOR VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[Y] = (unsigned short)(V[X] ^ V[Y]);
          pc += 2;
          break;
        }
        case 0x0004:{ //0x8XY4: add VX and VY into VX and save carry into VF
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;  
          if(V[X] > 255 - V[Y]){ //check for overflow
            V[0xF] = 1;
          }else{
            V[0xF] = 0;
          }
          V[X] = (unsigned short)((V[X] + V[Y]) & 0xFF); //truncate addition
          pc += 2;
          break;
        }
        case 0x0005:{ //0x8XY5: perform VX-VY and set VF to complimented carry
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          if (V[Y] > V[X]){
            V[0xF] = 0;
          }else{
            V[0xF] = 1;
          }
          V[X] = (unsigned short)(V[X] - V[Y]);
          pc += 2;
          break;
        }
        case 0x0006:{ //0x8XY6: shift VX 1 bit to the right (do not change VY)
                      //save LSB in VF before the shift
          int X = (opcode & 0x0F00) >> 8;
          V[0xF] = V[X] & 0x1;
          V[X] >>= 1;
          pc += 2;
          break;
        }
        case 0x0007:{//0x8XY7: perform VY-VX and set VF to complimented carry
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          if (V[X] > V[Y]){
            V[0xF] = 0;
          }else{
            V[0xF] = 1;
          }
          V[X] = (unsigned short)(V[Y] - V[X]);
          pc += 2;
          break;
        }
        case 0x000E:{//0x8XY6: shift VX 1 bit to the left (do not change VY)
                      //save MSB in VF before the shift
          int X = (opcode & 0x0F00) >> 8;
          V[0xF] = V[X] & 0x80;
          V[X] <<= 1;
          pc += 2;
          break;
        }
        default:
          cerr << "Opcode is not recognized!" << endl;
          exit(1);
      }
      break;
    }
 
    case 0x9000:{ // 0x9XY0: skip next instruction if VX != VY
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short Y = (opcode & 0x00F0) >> 4;
      if(this -> V[X] != this -> V[Y]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }
    case 0xA000:{
      this->I = (unsigned short)(opcode & 0x0FFF); //set address pointer to NNN
      pc += 2; //advance to next opcode
      break;
    }
    case 0xB000:{ //0xBNNN: jump to V0 + NNN
      int NNN = opcode & 0x0FFF;
      pc = (unsigned short)(NNN + V[0]);
      break;
    }
    case 0xC000:{ //0xCXNN: set V[X] to a random number & NN
      int X = (opcode & 0x0F00) >> 8;
      int NN = opcode & 0x00FF;
      V[X] = (unsigned short) (NN & rand()%256);
      pc += 2;
      break;
    }
    case 0xD000:{//DXYN: draw sprite of width 8px, height Npx, at coords (X,Y)
      int X = V[(opcode & 0x0F00) >> 8];
      int Y = V[(opcode & 0x00F0) >> 4];
      int height = opcode & 0x000F;
      V[0xF] = 0; //set collision flag to false by default
      int vert;
      int pixel;
      for(int i = 0; i < height; i++){
        vert = memory[I + i];
        for(int j = 0; j < 8; j++){
          pixel = vert & (0x80 >> j);
          if(pixel != 0){
            if(peripherals.gfx[Y + i][X + j] == 1){
              V[0xF] = 1; //collision detected
            }
            peripherals.gfx[Y + i][X + j] ^= 1;
          }
        }
      }
      peripherals.toUpdate = true;
      pc += 2;
      break;
    }

    case 0xE000:{
      switch(opcode & 0x00FF){
        case 0x009E:{ //0xEX9E: skip next instruction if key V[X] is pressed
          int key = V[(opcode & 0x0F00) >> 8];
          exit(0);
          if(this -> keyInputs[key] == 1){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }
        case 0x00A1:{//0xEXA1: skip next instruction if key V[X] is not pressed
          int key = V[(opcode & 0x0F00) >> 8];
          //cout << "keypressed: " << key << endl;

          if(this -> keyInputs[key] == 0){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }    
        default:{
          cerr << "Opcode is not recognized!" << endl;
          exit(1); 
        }  
      }
      break;
    }

    case 0xF000:{
      switch(opcode & 0x00FF){ //0xFX33: store a binary-coded decimal V[X]'s digits in I, I+1, I+2
        case 0x0007:{ //0xFX07 : set V[x] to delay timer
          V[(opcode & 0x0F00) >> 8] = (unsigned short) this -> delay_timer;
          pc += 2;
          //break;
        }
        case 0x0015:{ //0xFX15: set delay timer to V[x]
          this -> delay_timer = V[(opcode & 0x0F00) >> 8];
          pc += 2;
          //break;
        } 
        case 0x0029:{ //0xFX29: sets I to point to character sprite in V[X]
          int character = V[(opcode & 0x0F00)  >> 8];
          I = (unsigned short)(0x50 + 5 * character);
          pc += 2;
          break;
        }
        case 0x0033:{
          int mem = V[(opcode & 0x0F00) >> 8];
          memory[I] = (unsigned short)mem/100; //hundreds
          memory[I+1] = (unsigned short)(mem/10)%10; //tens
          memory[I+2] = (unsigned short)(mem%10); //ones
          pc += 2;
          break;
        }
        case 0x0065:{ //0xFX65: loads data to register V[0] to V[X] starting from mem address I
          int bound = (opcode & 0x0F00) >> 8;
          for(int i = 0; i < bound; i++){
            V[i] = memory[I+i];
          }
          this -> I = (unsigned short)(this -> I + bound + 1);
          pc += 2;
          break;
        }
        default:{
          cerr << "Opcode is not recognized!" << endl;
          exit(1);
        }
      }
      break;
    }
  }
}

int main(int argc, char* argv[]){
  if (!al_init()) {
		cerr << "Failed to initialize allegro." << endl;
		return 1;
	}

  Chip8Cpu* cpu = new Chip8Cpu();
  Peripherals* peripherals = new Peripherals();

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / peripherals->FPS);
  if (!timer) {
		cerr << "Failed to create timer." << endl;
	}

  if(!al_install_keyboard()) {
    cerr << "failed to initialize the keyboard!" << endl;
    return -1;
  }

  al_register_event_source(peripherals->event_queue, al_get_timer_event_source(timer));

  cpu -> loadFont();

  if(!cpu -> loadProgram((char *)"../roms/pong2.c8")){
    return -1;
  }
  
  al_start_timer(timer);

  al_register_event_source(peripherals -> event_queue, al_get_keyboard_event_source());

  while (peripherals->running){
    
    if (al_wait_for_event_until(peripherals->event_queue, &(peripherals->event), &(peripherals->timeout))) {
      switch (peripherals->event.type) {
        case ALLEGRO_EVENT_TIMER:{
          break;
        }
        case ALLEGRO_EVENT_KEY_DOWN:{
          switch(peripherals->event.keyboard.keycode) {
            case ALLEGRO_KEY_1:
              cpu -> setKey(1);
              cout << "                    1" << endl;
              break;

            case ALLEGRO_KEY_2:
              cpu -> setKey(2);
              cout << "                    2" << endl;
              break;

            case ALLEGRO_KEY_3: 
              cpu -> setKey(3);
              cout << "                    3" << endl;
              break;

            case ALLEGRO_KEY_4:
              cpu -> setKey(4);
              cout << "                    4" << endl;
              break;

            case ALLEGRO_KEY_5:
              cpu -> setKey(5);
              cout << "                    5" << endl;
              break;

            case ALLEGRO_KEY_6:
              cpu -> setKey(6);
              cout << "                    6" << endl;
              break;

            case ALLEGRO_KEY_7: 
              cpu -> setKey(7);
              cout << "                    7" << endl;
              break;

            case ALLEGRO_KEY_8:
              cpu -> setKey(8);
              cout << "                    8" << endl;
              break;
            
            case ALLEGRO_KEY_9:
              cpu -> setKey(9);
              cout << "                    9"<< endl;
              break;
            
            case ALLEGRO_KEY_Q:
              cpu -> setKey(0);
              cout << "                    Q"<< endl;
              break;

            case ALLEGRO_KEY_W:
              cpu -> setKey(10);
              cout << "                    W"<< endl;
              break;

            case ALLEGRO_KEY_E: 
              cpu -> setKey(11);
              cout << "                    E"<< endl;
              break;

            case ALLEGRO_KEY_R:
              cpu -> setKey(12);
              cout << "                    R"<< endl;
              break;

            case ALLEGRO_KEY_T:
              cpu -> setKey(13);
              cout << "                    T"<< endl;
              break;

            case ALLEGRO_KEY_Y:
              cpu -> setKey(14);
              cout << "                    Y"<< endl;
              break;

            case ALLEGRO_KEY_U: 
              cpu -> setKey(15);
              cout << "                    U"<< endl;
              break;
          }
          break;
        }           

        case ALLEGRO_EVENT_DISPLAY_CLOSE:{
          peripherals->running = false;
          break;
        }
        default:{
          break;
        }
      }
    }
    cpu -> run(*peripherals);
    peripherals -> updateDisplay();
    
    cpu -> resetKeys();
    usleep(10000);
  }
  al_destroy_timer(timer);
  return 0;
}






