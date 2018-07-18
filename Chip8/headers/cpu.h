#ifndef CPU_H
#define CPU_H

#include "peripherals.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>

class Chip8Cpu{
  private:
    unsigned short *memory;
    unsigned short *V;       
    unsigned short I;          
    unsigned short pc;           
    unsigned char *keyInputs;
    unsigned short *stack; 
    unsigned int sp;

    

  public:
    int delay_timer;  
    int sound_timer;
    bool running;
    Chip8Cpu();
    void run(Peripherals& peripherals);
    void loadFont();
    void resetKey(int ind);
    void setKey(int ind);
    bool loadProgram(char* fileName);
    ~Chip8Cpu();
};

#endif
