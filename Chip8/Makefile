LIB_DIRS=$(SYSTEMC_HOME)/lib-macosx64

INCLUDE_DIRS = -I. -I$(SYSTEMC_HOME)/include

HEADERS = fir.h tb.h
SOURCES = main.cc fir.cc tb.cc

DEPENDANCIES = \
	Makefile \
	$(HEADERS) \
	$(SOURCES)

LIBS = -lsystemc -lstdc++ -lm

TESTS = main


all: $(TESTS)
	./$(TESTS)
	@make cmp_result

$(TESTS): $(DEPENDANCIES)
	gcc -g -o $@ $(SOURCES) $(INCLUDE_DIRS) -L$(LIB_DIRS) $(LIBS)

clean:
	rm -f $(TESTS) *.dat

GOLD_DIR = ./golden
GOLD_FILE = $(GOLD_DIR)/ref_output.dat

cmp_result:
	@echo "-----------------------------------------"
	@if diff -w $(GOLD_FILE) ./output.dat ; then\
		echo "SIMULATION PASSED"; \
	else\
		echo "SIMULATION FAILED"; \
	fi
	@echo "-----------------------------------------"

#################################################################

SDL_LIB_DIRS = $(SDL_HOME)/lib

SDL_INCLUDE_DIRS = -I. -I$(SDL_HOME)/include

SDL_HEADERS = peripherals.h
SDL_SOURCES = peripherals.cc

SDL_LIBS = -lSDL2-2.0.0

SDL_EXEC = gfx

SDL_DEPENDANCIES = \
	Makefile \
	$(SDL_HEADERS) \
	$(SDL_SOURCES)

gfx: $(SDL_DEPENDANCIES)
	gcc -g -o $@ $(SDL_SOURCES) $(SDL_INCLUDE_DIRS) -L$(SDL_LIB_DIRS) $(SDL_LIBS)