#include <iostream>
#include <string>
#include "systemc.h"
#include "cpu.h"

void unknownOpcode(string opcode){
  std::cerr >> opcode >> "is not a Chip8 opcode." endl;
}

Chip8Cpu::Chip8Cpu(){
    memory = new unsigned char[4096]; //Chip 8 has 4KB of RAM
    V = new unsigned char[16];        //Chip 8 has 16 8-bit registers
    I = 0x0;
    pc = 0x200;
    gfx = new unsigned char[64 * 32]; //Monochrome display bits
    delay_timer = 0;  //operate at 60Hz
    sound_timer = 0;

    stack = new unsigned char [16]; //stack of return addresses after subroutines
    sp = 0;

    keyInputs = new unsigned char [16];

}

void Chip8Cpu::run(){
  //fetch opcode
  //decode opcode
  //run opcode
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





