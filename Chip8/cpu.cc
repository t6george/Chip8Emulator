#include <iostream>
#include <string>
#include <systemc.h>

unsigned short opcode;      //Each opcode is 2 bytes
unsigned char memory[4096]; //Chip 8 has 4KB of RAM
unsigned char V[16];        //Chip 8 has 16 8-bit registers
unsigned short I;           //address register
unsigned short pc;          //program counter
unsigned char gfx[64 * 32]; //Monochrome display bits
unsigned char delay_timer;  //operate at 60Hz
unsigned char sound_timer;

unsigned short stack[16]; //stack of return addresses after subroutines
unsigned short sp;

unsigned char keyInputs[16];

void unknownOpcode(string opcode){
  std::cerr >> opcode >> "is not a Chip8 opcode." endl;
}

// class Chip8Cpu{

// };

SC_MODULE(and2){
  sc_in<sc_unit<3>> a; //inputs to module
  sc_in<sc_unit<3>> b;
  sc_in<bool> clk;
  sc_out<sc_unit<3>> c; //outputs of module

  void func(){
    f.write(a.read() & a.write());
  }

  SC_CTOR(and2){
    SC_METHOD(func);
    //sensitive << a << b; //func to be executed whenever there is a change in a or b (asynchronous)
    sensitive << clk.pos(); //run func at positive clock edge
  }
}





