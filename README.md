![Pong](https://imgur.com/DrmBm2v)
![Space Invaders](https://imgur.com/OGiYEV8)
![Tetris](https://imgur.com/30Vc2Jn)

# Chip8 Emulator
Chip8 is an interpreted programming language developed in the mid 1970s, and was initially used to program simple games for the COSMAC VIP. The Chip8's library included many classics, such as Pong, Pacman, Tetris, Space Invaders, Connect 4, and Syzgy.

This emulator was created in C++, with its graphics rendered using the Allegro Library. There is a seperate make rule for if you want to play the Chip8 on a Raspberry Pi connected to a 64x32 LED Matrix (using this open-source library: [https://github.com/hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)). 

*make gfx rom=romName (Compiles with allegro libraries)
*make mtx rom=romName (Compiles for LED Matrix)

Note: If you do not specify a romName, the emulator will default to Pong.