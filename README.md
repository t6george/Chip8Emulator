![Pong](https://user-images.githubusercontent.com/31244240/43002878-662c4d66-8bf8-11e8-9f4d-d7cd07b85819.png)
![Space Invaders](https://user-images.githubusercontent.com/31244240/43002916-87f29b4e-8bf8-11e8-8378-50bcf2737597.png)
![Tetris](https://user-images.githubusercontent.com/31244240/43002909-7a69a576-8bf8-11e8-9dc4-c29bea3c1318.png)

# Chip8 Emulator
Chip8 is an interpreted programming language developed in the mid 1970s, and was initially used to program simple games for the COSMAC VIP. The Chip8's library included many classics, such as Pong, Pacman, Tetris, Space Invaders, Connect 4, and Syzgy.

This emulator was created in C++, with its graphics rendered using the Allegro Library. There is a seperate make rule for if you want to play the Chip8 on a Raspberry Pi connected to a 64x32 LED Matrix (using this open-source library: [https://github.com/hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix)). 

* make gfx rom=romName (Compiles with allegro libraries)
* make mtx rom=romName (Compiles for LED Matrix)

Note: If you do not specify a romName, the emulator will default to Pong.
