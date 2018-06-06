#ifndef CPU_H
#define CPU_H

class Chip8Cpu{
  unsigned short *memory;
  unsigned short *V;       
  unsigned short I;          
  unsigned short pc;           
  int delay_timer;  
  int sound_timer;

  unsigned short *stack; 
  unsigned int sp;

  unsigned char *keyInputs;

  Chip8Cpu();
  void run();
  ~Chip8Cpu();
};

#endif
