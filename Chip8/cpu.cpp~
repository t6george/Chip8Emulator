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
