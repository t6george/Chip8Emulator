#ifndef CPU_H
#define CPU_H

#include "peripherals.h"


class Chip8Cpu{
  private:
    unsigned short *memory;
    unsigned short *V;       
    unsigned short I;          
    unsigned short pc;           
    int delay_timer;  
    int sound_timer;

    unsigned short *stack; 
    unsigned int sp;

    unsigned char *keyInputs;

    Peripherals *peripherals;

  public:
    Chip8Cpu();
    void run();
    void kernel();
    void loadFont();
    bool loadProgram(char* fileName);
    ~Chip8Cpu();
};

#endif
