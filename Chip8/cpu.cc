#include "cpu.h"

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
  //char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);
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
 cout << "opcode: " << hex << opcode << endl;
//  if(opcode == 0xEE){
//    peripherals.running = false;
//  }
 switch(opcode & 0xF000){
  case 0x0000:
    switch(opcode & 0x00FF){
      case 0x00E0: //clear the screen 
        break;
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

  case 0x1000:{ //0x1NNN: jump to NNN
    pc = (unsigned short)(opcode & 0x0FFF);
    break;
  }
  case 0x2000:{
    unsigned short address = (unsigned short)opcode & 0x0FFF;
    stack[sp] = pc; //save where program counter was at on the stack
    sp++; //increment stack
    pc = address; //jumpt to NNN: address of desired subroutine
    break;
  }

  case 0x3000:{ //0x3XNN: skips next opcode if VX == NN
    unsigned short x = (opcode & 0x0F00) >> 8;
    unsigned short nn = opcode & 0x00FF;
    if(this -> V[x] == nn){
      pc += 4;
    }else{
      pc += 2;
    }   
    break;
  }
  case 0x4000:
    break;

  case 0x5000:
    break;

  case 0x6000:{
    int ind = (opcode & 0x0F00) >> 8;
    V[ind] = (unsigned short)(opcode & 0x0FF); //set register N to NN (8 bits)
    pc += 2; //advance program counter to next opcode
    break;
  }
  case 0x7000:{
    int ind = (opcode & 0x0F00) >> 8;
    int toAdd = opcode & 0x00FF;
    V[ind] = (unsigned short)((V[ind] + toAdd) & 0xFF);
    pc += 2;
    break;
  }

  case 0x8000:
    switch(opcode & 0x000F){
      case 0x0000:
        break;

      case 0x0001:
        break;

      case 0x0002:
        break;

      case 0x0003:
        break;

      case 0x0004:
        break;

      case 0x0005:
        break;

      case 0x0006:
        break;

      case 0x0007:
        break;
      
      case 0x000E:
        break;
    
      default:
        cerr << "Opcode is not recognized!" << endl;
        exit(1);
    }
    break;
  case 0x9000:
    break;

  case 0xA000:{
    this->I = (unsigned short)(opcode & 0x0FFF); //set address pointer to NNN
    pc += 2; //advance to next opcode
    break;
  }
  case 0xB000:
    break;

  case 0xC000:
    break;

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

  case 0xE000:
    switch(opcode & 0x000F){
      case 0x000E:
        break;
      case 0x0001:
        break;
      default:
        cerr << "Opcode is not recognized!" << endl;
      exit(1);   
    }
    break;

  case 0xF000:
    switch(opcode & 0x000F){
      case 0x0007:
        break;

      case 0x000A:
        break;

      case 0x0005:
        switch(opcode & 0x00F0){
          case 0x0010:
            break;
          case 0x0050:
            break;
          case 0x0060:
            break;

          default:
            cerr << "Opcode is not recognized!" << endl;
            exit(1);
        }
        break;

      case 0x0008:
        break;

      case 0x000E:
        break;

      case 0x0009:
        break;

      case 0x0003:
        break;

      default:
        cerr << "Opcode is not recognized!" << endl;
        exit(1);
    }
  
    break;

  default:
    cerr << "Opcode is not recognized!" << endl;
    exit(1);
  }

}

void Chip8Cpu::kernel(){
  //multithread here
}

int main(int argc, char* argv[]){
  if (!al_init()) {
		cerr << "Failed to initialize allegro." << endl;
		return 1;
	}

  Chip8Cpu* cpu = new Chip8Cpu();
  Peripherals* peripherals = new Peripherals();

  cpu -> loadFont();
  cpu -> loadProgram((char *)"pong2.c8");
  
  while (peripherals->running){
    peripherals -> get_event = al_wait_for_event_until(peripherals->event_queue, &(peripherals->event), &(peripherals->timeout));
    if (peripherals -> get_event) {
      switch (peripherals->event.type) {
        case ALLEGRO_EVENT_TIMER:{
          break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE:{
          peripherals->running = false;
          break;
        }
        default:{
          cerr << "Unsupported event received." << endl;
          break;
        }
      }
    }
    
    peripherals -> updateDisplay();
    cpu -> run(*peripherals);
    //usleep(1000000);
  }
  return 0;
}

// SC_MODULE(and2){
//   sc_in< sc_unit<3> > a; //inputs to module
//   sc_in< sc_unit<3> > b;
//   sc_in<bool> clk;
//   sc_out< sc_unit<3> > c; //outputs of module

//   void func(){
//     f.write(a.read() & a.write());
//   }

//   SC_CTOR(and2){
//     SC_METHOD(func);
//     //sensitive << a << b; //func to be executed whenever there is a change in a or b (asynchronous)
//     sensitive << clk.pos(); //run func at positive clock edge
//   }
// }






