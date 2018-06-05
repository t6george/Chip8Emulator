class Chip8Cpu{
  unsigned short *memory;
  unsigned short *V;       
  unsigned short I;          
  unsigned short pc;          
  unsigned char *gfx; 
  int delay_timer;  
  int sound_timer;

  unsigned short *stack; 
  unsigned int sp;

  unsigned char *keyInputs;

  Chip8Cpu();
  void run();
};
