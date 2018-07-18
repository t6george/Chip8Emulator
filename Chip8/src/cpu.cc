#include "../headers/cpu.h"

using namespace std;

Chip8Cpu::Chip8Cpu(){
    this -> memory = new unsigned short[0x1000];                      //4KB of program memory
    this -> V = new unsigned short[16];                               //16 8-bit registers
    this -> I = 0x0;                                                  //memory address register default
    pc = 0x0200;                                                      //load rom at this starting address
    this -> delay_timer = 0;                        
    this -> sound_timer = 0;

    this -> stack = new unsigned short[16];                           //stack of return addresses after subroutines
    this -> sp = 0;                                                   //stack initially is empty

    this -> keyInputs = new unsigned char[16];
    for(int i = 0; i < 16; i++){
      this -> keyInputs[i] = 0;
    }

    this -> running = true;
}

void Chip8Cpu::resetKey(int ind){
  this->keyInputs[ind] = 0;                                   //set key to unpressed state
}

void Chip8Cpu::setKey(int ind){
  this->keyInputs[ind] = 1;                                   //set key to pressed state
}

Chip8Cpu::~Chip8Cpu(){
  delete [] memory;
  delete [] V;
  delete [] stack;
  delete [] keyInputs;
}

bool Chip8Cpu::loadProgram(char* fileName){
  cout << "Loading ROM..." << endl;

  FILE* rom = fopen(fileName, "rb");                          //open binary rom file for read only
  if (nullptr == rom) {
    cerr << "Failed to open ROM" << endl;
    return false;
  }
  fseek(rom, 0, SEEK_END);                                    //move position indicator to eof
  long rom_size = ftell(rom);                                 //get position indicator (which is file size)
  rewind(rom);                                                //reset position indicator back to the beginning
                                          
  char* rom_buffer = new char[rom_size];                      //Allocate memory to store rom

  if (rom_buffer == NULL) {
      cerr << "Failed to allocate memory for ROM" << endl;
      return false;
  }

  size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom); //read and store rom in temporary buffer
  
  if (result != rom_size) {
      cerr << "Failed to read ROM" << endl;
      return false;
  }

  
  if ((0x1000-0x200) > rom_size){                             // Copy buffer to memory
      for (int i = 0; i < rom_size; ++i) {
          memory[0x0200 + i] = (uint8_t)rom_buffer[i];   
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
    memory[0x50 + i] = (unsigned short)((fontset[i]) & 0xFF);  //rom expects fonts to be loaded at 0x50 and onward
  }
  cout << "Fontset is loaded" << endl;
}

void Chip8Cpu::run(Peripherals& peripherals){
  unsigned int opcode = (unsigned int)memory[pc] << 8 | (unsigned int)memory[pc+1];
  cout << "opcode: 0x" << hex << opcode << endl;

  switch(opcode & 0xF000){
    case 0x0000:{
      switch(opcode & 0x00FF){

        case 0x00E0:{                                         //0x00E0: clear the screen 
          for(int i = 0; i < 32; i++){
            for(int j = 0; j < 64; j++){
              peripherals.gfx[i][j] = 0;
            }
          }
          pc += 2;
          break;
        }

        case 0x00EE:{                                         //0x00EE: return from subroutine
          sp--;
          pc = (unsigned short)(stack[sp]+2);
          break;
        }

        default:{
          cerr << "----------------Unknown Opcode----------------" << endl;
          exit(127);
          break;
        }

      }
      break;
    }

    case 0x1000:{                                           //0x1NNN: jump to NNN
      pc = (unsigned short)(opcode & 0x0FFF);
      break;
    }

    case 0x2000:{                                           //0x2NNN: call subroutine at address NNN
      stack[sp] = pc;                                       //save where program counter was at on the stack
      sp++;                             
      pc = (unsigned short)(opcode & 0x0FFF);               //jump to NNN: address of desired subroutine
      break;
    }

    case 0x3000:{                                           //0x3XNN: skips next opcode if VX == NN
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if(this -> V[X] == NN){
        pc += 4;
      }else{
        pc += 2;
      }   
      break;
    }

    case 0x4000:{                                           //0x4XNN: skip the next instruction if VX != NN
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short NN = opcode & 0x00FF;
      if(this -> V[X] != NN){
        pc += 4;
      }else{
        pc += 2;
      }   
      break;
    }

    case 0x5000:{                                           //0x5XY0: skip next instruction if VX == VY
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short Y = (opcode & 0x00F0) >> 4;
      if(this -> V[X] == this -> V[Y]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }

    case 0x6000:{                                           //0x6XNN: set VX to NN
      int X = (opcode & 0x0F00) >> 8;
      V[X] = (unsigned short)(opcode & 0x00FF);       
      pc += 2; 
      break;
    }

    case 0x7000:{                                          //0x7XNN: adds NN to VX
      int X = (opcode & 0x0F00) >> 8;
      int NN = opcode & 0x00FF;
      V[X] = (unsigned short)((V[X] + NN) & 0xFF);
      pc += 2;
      break;
    }

    case 0x8000:{
      switch(opcode & 0x000F){

        case 0x0000:{                                       //0x8XY0: set VX to VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = V[Y];
          pc += 2;
          break;
        }

        case 0x0001:{                                       //0x8XY1: set VX to VX | VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = (unsigned short)(V[X] | V[Y]);
          pc += 2;
          break;
        }

        case 0x0002:{                                       //0x8XY2: sets VX to VX & VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = (unsigned short)(V[X] & V[Y]);
          pc += 2;
          break;
        }

        case 0x0003:{                                       //0x8XY3: set VX to VX ^ VY
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          V[X] = (unsigned short)(V[X] ^ V[Y]);
          pc += 2;
          break;
        }

        case 0x0004:{                                       //0x8XY4: add VX and VY into VX and save carry into VF
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;  
          if(V[X] > 0xFF - V[Y]){                           //check for overflow
            this -> V[0xF] = 1;
          }else{
            this -> V[0xF] = 0;
          }
          this -> V[X] = (unsigned short)((V[X] + V[Y]) & 0xFF); //truncate addition to 8 bits
          pc += 2;
          break;
        }

        case 0x0005:{                                       //0x8XY5: perform VX-VY and set VF to complimented carry
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          if (V[Y] > V[X]){
            this -> V[0xF] = 0;
          }else{
            this -> V[0xF] = 1;
          }
          this -> V[X] = (unsigned short)(V[X] - V[Y]);
          pc += 2;
          break;
        }

        case 0x0006:{                                       //0x8XY6: shift VX 1 bit to the right (do not change VY)
                                                            //save LSB in VF before the shift
          int X = (opcode & 0x0F00) >> 8;
          this -> V[0xF] = V[X] & 0x1;
          this -> V[X] >>= 1;
          pc += 2;
          break;
        }

        case 0x0007:{                                       //0x8XY7: perform VY-VX and set VF to complimented carry
          int X = (opcode & 0x0F00) >> 8;
          int Y = (opcode & 0x00F0) >> 4;
          if (this -> V[X] > this -> V[Y]){
            this -> V[0xF] = 0;
          }else{
            this -> V[0xF] = 1;
          }
          this -> V[X] = (unsigned short)(this -> V[Y] - this -> V[X]);
          pc += 2;
          break;
        }

        case 0x000E:{                                       //0x8XY6: shift VX 1 bit to the left (do not change VY)
                                                            //save MSB in VF before the shift
          int X = (opcode & 0x0F00) >> 8;
          this -> V[0xF] = this -> V[X] >> 7;
          this -> V[X] <<= 1;
          pc += 2;
          break;
        }
        default:
          cerr << "----------------Unknown Opcode----------------" << endl;
          exit(127);
      }
      break;
    }
 
    case 0x9000:{                                          //0x9XY0: skip next instruction if VX != VY
      unsigned short X = (opcode & 0x0F00) >> 8;
      unsigned short Y = (opcode & 0x00F0) >> 4;
      if(this -> V[X] != this -> V[Y]){
        pc += 4;
      }else{
        pc += 2;
      }
      break;
    }

    case 0xA000:{                                           //0xANNN: set address register to NNN
      this->I = (unsigned short)(opcode & 0x0FFF);          
      pc += 2; 
      break;
    }

    case 0xB000:{                                           //0xBNNN: jump to V0 + NNN
      int NNN = opcode & 0x0FFF;
      pc = (unsigned short)(NNN + V[0]);
      break;
    }

    case 0xC000:{                                           //0xCXNN: set V[X] to a random number & NN
      int X = (opcode & 0x0F00) >> 8;
      int NN  = opcode & 0x00FF;
      this -> V[X] = (unsigned short) (NN & rand()%256);
      pc += 2;
      break;
    }
    case 0xD000:{                                           //DXYN: draw sprite of width 8px, height Npx, at coords (VX,VY)
      int X = V[(opcode & 0x0F00) >> 8];
      int Y = V[(opcode & 0x00F0) >> 4];
      int height = opcode & 0x000F;
      this -> V[0xF] = 0;                                             //set collision flag to false by default
      int sprite;
      int pixel;
      for(int i = 0; i < height; i++){
        sprite = this -> memory[I + i];
        for(int j = 0; j < 8; j++){
          pixel = sprite & (0x80 >> j);                               //get each column of the sprite
          if(pixel != 0){
            if(peripherals.gfx[(Y + i)%32][(X + j)%64] == 1){        
              this -> V[0xF] = 1;                                     //collision detected
            }
            peripherals.gfx[(Y + i)%32][(X + j)%64] ^= 1;             //toggle the pixel being drawn (screen-wrapping)
          }
        }
      }
      peripherals.toUpdate = true;
      pc += 2;
      break;
    }

    case 0xE000:{
      switch(opcode & 0x00FF){

        case 0x009E:{                                       //0xEX9E: skip next instruction if key V[X] is pressed
          int key = this -> V[(opcode & 0x0F00) >> 8];
          if(this -> keyInputs[key] == 1){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }

        case 0x00A1:{                                       //0xEXA1: skip next instruction if key V[X] is not pressed
          int key = this -> V[(opcode & 0x0F00) >> 8];
          if(this -> keyInputs[key] == 0){
            pc += 4;
          }else{
            pc += 2;
          }
          break;
        }    
        default:{
          cerr << "----------------Unknown Opcode----------------" << endl;
          exit(127); 
        }  
      }
      break;
    }

    case 0xF000:{
      switch(opcode & 0x00FF){                            
        
        case 0x0007:{                                     //0xFX07 : set V[x] to delay timer
          this -> V[(opcode & 0x0F00) >> 8] = (unsigned short) this -> delay_timer;
          pc += 2;
          break;
        }
        
        case 0x0015:{                                     //0xFX15: set delay timer to V[x]
          this -> delay_timer = V[(opcode & 0x0F00) >> 8];
          pc += 2;
          break;
        } 
        
        case 0x0029:{                                     //0xFX29: sets I to point to character sprite in V[X]
          int character = V[(opcode & 0x0F00)  >> 8];
          this -> I = (unsigned short)(0x50 + 5 * character);
          pc += 2;
          break;
        }
        
        case 0x000A:{                                     //0xFX0A: await a keypress and store in VX
          int X = (opcode & 0x0F00) >> 8;
          bool pressed = false;
          for(int i = 0; i < 16; i++){
            if(this -> keyInputs[i] == 1){
              pressed = true;
              this -> V[X] = i;
            }
          }
          if(!pressed){
            return;
          }
          pc += 2;
          break;
        }
        
        case 0x001E:{                                    //0xFX1E: set I to VX + I and VF is set to the overflow
          int X = (opcode & 0x0F00) >> 8;
          if(this -> V[X] + this -> I > 0x0FFF){
            this -> V[0xF] = 1;
          }else{
            this -> V[0xF] = 0;
          }
          this -> I = this -> I + V[X];
          pc += 2;
          break;
        }
        
        case 0x0033:{                                   //0xFX33: store a binary-coded decimal V[X]'s digits in I, I+1, I+2
          int mem = this -> V[(opcode & 0x0F00) >> 8];       
          this -> memory[I] = (unsigned short)mem/100;          //hundreds
          this -> memory[I+1] = (unsigned short)(mem/10)%10;    //tens
          this -> memory[I+2] = (unsigned short)(mem%10);       //ones
          pc += 2;
          break;
        }
        
        case 0x0055:{                                   //0xFX55: stores V0 to VX in memory starting at address I
          int X = (opcode & 0x0F00) >> 8;
          for(int i = 0; i < X; i++){
            this->memory[this -> I+i] = this -> V[i];
          }
          this -> I = this -> I + X + 1;
          pc += 2;
          break;
        }
        
        case 0x0065:{                                   //0xFX65: loads data to register V[0] to V[X] starting from mem address I
          int bound = (opcode & 0x0F00) >> 8;
          for(int i = 0; i <= bound; i++){
            this -> V[i] = this -> memory[I+i];
          }
          this -> I = (unsigned short)(this -> I + bound + 1);
          pc += 2;
          break;
        }
        
        case 0x0018:{                                   //0xFX15: set sound timer to VX
          int X = (opcode & 0x0F00) >> 8;
          this -> sound_timer = this -> V[X];
          pc += 2;
          break;
        }
        
        default:{
          cerr << "----------------Unknown Opcode----------------" << endl;
          exit(127);
        }
      }
      break;
    }
  }
}

int main(int argc, char* argv[]){
  al_init();

  Chip8Cpu* cpu = new Chip8Cpu();
  Peripherals* peripherals = new Peripherals(strncmp(argv[1],"gfx",3));

  cpu -> loadFont();

  cpu -> loadProgram((char *)"../roms/pong2.c8");

  al_install_keyboard();
  al_register_event_source(peripherals -> event_queue, al_get_keyboard_event_source());

  while (cpu->running){
    
    if (al_wait_for_event_until(peripherals->event_queue, &(peripherals->event), &(peripherals->timeout))) {
      switch (peripherals->event.type) {
        case ALLEGRO_EVENT_KEY_DOWN:{
          switch(peripherals->event.keyboard.keycode) {
            case ALLEGRO_KEY_1:
              cpu -> setKey(0x1);
              break;

            case ALLEGRO_KEY_2:
              cpu -> setKey(0x2);
              break;

            case ALLEGRO_KEY_3: 
              cpu -> setKey(0x3);
              break;

            case ALLEGRO_KEY_4:
              cpu -> setKey(0x4);
              break;

            case ALLEGRO_KEY_5:
              cpu -> setKey(0x5);
              break;

            case ALLEGRO_KEY_6:
              cpu -> setKey(0x6);
              break;

            case ALLEGRO_KEY_7: 
              cpu -> setKey(0x7);
              break;

            case ALLEGRO_KEY_8:
              cpu -> setKey(0x8);
              break;
            
            case ALLEGRO_KEY_9:
              cpu -> setKey(0x9);
              break;
            
            case ALLEGRO_KEY_Q:
              cpu -> setKey(0x0);
              break;

            case ALLEGRO_KEY_W:
              cpu -> setKey(0xA);
              break;

            case ALLEGRO_KEY_E: 
              cpu -> setKey(0xB);
              break;

            case ALLEGRO_KEY_R:
              cpu -> setKey(0xC);
              break;

            case ALLEGRO_KEY_T:
              cpu -> setKey(0xD);
              break;

            case ALLEGRO_KEY_Y:
              cpu -> setKey(0xE);
              break;

            case ALLEGRO_KEY_U: 
              cpu -> setKey(0xF);
              break;
          }
          break;
        }           
        case ALLEGRO_EVENT_KEY_UP:{
          switch(peripherals->event.keyboard.keycode) {
            case ALLEGRO_KEY_1:
              cpu -> resetKey(0x1);
              break;

            case ALLEGRO_KEY_2:
              cpu -> resetKey(0x2);
              break;

            case ALLEGRO_KEY_3: 
              cpu -> resetKey(0x3);
              break;

            case ALLEGRO_KEY_4:
              cpu -> resetKey(0x4);
              break;

            case ALLEGRO_KEY_5:
              cpu -> resetKey(0x5);
              break;

            case ALLEGRO_KEY_6:
              cpu -> resetKey(0x6);
              break;

            case ALLEGRO_KEY_7: 
              cpu -> resetKey(0x7);
              break;

            case ALLEGRO_KEY_8:
              cpu -> resetKey(0x8);
              break;
            
            case ALLEGRO_KEY_9:
              cpu -> resetKey(0x9);
              break;
            
            case ALLEGRO_KEY_Q:
              cpu -> resetKey(0x0);
              break;

            case ALLEGRO_KEY_W:
              cpu -> resetKey(0xA);
              break;

            case ALLEGRO_KEY_E: 
              cpu -> resetKey(0xB);
              break;

            case ALLEGRO_KEY_R:
              cpu -> resetKey(0xC);
              break;

            case ALLEGRO_KEY_T:
              cpu -> resetKey(0xD);
              break;

            case ALLEGRO_KEY_Y:
              cpu -> resetKey(0xE);
              break;

            case ALLEGRO_KEY_U: 
              cpu -> resetKey(0xF);
              break;
          }
          break;
        }  
        case ALLEGRO_EVENT_DISPLAY_CLOSE:{
          cpu->running = false;
          break;
        }
        default:{
          break;
        }
      }
    }
    cpu -> run(*peripherals);

    if(NULL != peripherals -> display && peripherals -> toUpdate){
      peripherals -> updateDisplay();
    }
    // else if(NULL != peripherals -> canvas && peripherals -> toUpdate){
    //   peripherals -> updateLEDMatrix(peripherals -> canvas);
    // }
  
    if (cpu -> sound_timer > 0){
      cpu -> sound_timer--;
    }
    if (cpu -> delay_timer > 0){
      cpu -> delay_timer--;
    }
    usleep(4000);
  }
  return 0;
}






