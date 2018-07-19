#------------------------------------Allegro5 Build------------------------------------#
export gfx_HOME=/usr/local/Cellar/allegro/5.2.4.0

gfx_LIB_DIRS = $(gfx_HOME)/lib

gfx_INCLUDE_DIRS = -I. -I$(gfx_HOME)/include/allegro5

gfx_HEADERS = ../headers/peripherals.h ../headers/cpu.h
gfx_SOURCES = peripherals.cc cpu.cc

gfx_LIBS = -lallegro -lallegro_main -lallegro_primitives.5.2 -lstdc++

gfx_EXEC = gfx

gfx_DEPENDANCIES = \
	Makefile \
	$(gfx_HEADERS) \
	$(gfx_SOURCES)

gfx: $(gfx_DEPENDANCIES)
	gcc -g -o $@ $(gfx_SOURCES) $(gfx_INCLUDE_DIRS) -L$(gfx_LIB_DIRS) $(gfx_LIBS)
	./$(gfx_EXEC) gfx $(rom)

#------------------------------------LED MATRIX Build------------------------------------#
mtx_LIB_DIRS = /home/pi/rpi-rgb-led-matrix/lib

mtx_INCLUDE_DIRS = -I. -I/home/pi/rpi-rgb-led-matrix/include

mtx_HEADERS := $(gfx_HEADERS)
mtx_SOURCES := $(gfx_SOURCES)

mtx_LIBS = -lrgbmatrix -lrt -lm -lpthread

mtx_EXEC = mtx

gfx_DEPENDANCIES = \
	Makefile \
	$(mtx_HEADERS) \
	$(mtx_SOURCES)

mtx: $(mtx_DEPENDANCIES)
	gcc -g -o $@ $(mtx_SOURCES) $(mtx_INCLUDE_DIRS) -L$(mtx_LIB_DIRS) $(mtx_LIBS)
	./$(mtx_EXEC) mtx

clean:
	rm -f gfx
	rm -f mtx