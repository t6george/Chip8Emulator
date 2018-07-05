#ifndef CPU_H
#define CPU_H

#include "peripherals.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

  public:
    Chip8Cpu();
    void run(Peripherals& peripherals);
    void loadFont();
    void resetKeys();
    void setKey(int ind);
    bool loadProgram(char* fileName);
    ~Chip8Cpu();
};

#endif
