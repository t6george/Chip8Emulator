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

gfx: $(gfxL_DEPENDANCIES)
	gcc -g -o $@ $(gfx_SOURCES) $(gfx_INCLUDE_DIRS) -L$(gfx_LIB_DIRS) $(gfx_LIBS)
	./$(gfx_EXEC)