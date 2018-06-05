class Chip8Cpu{
  unsigned char *memory;
  unsigned char *V;       
  unsigned short I;          
  unsigned short pc;          
  unsigned char *gfx; 
  unsigned char delay_timer;  
  unsigned char sound_timer;

  unsigned char *stack; 
  unsigned int sp;

  unsigned char *keyInputs;

  Chip8Cpu();
  void run();
};
