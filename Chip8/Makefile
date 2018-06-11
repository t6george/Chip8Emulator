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

export gfx_HOME=/usr/local/Cellar/allegro/5.2.4.0

gfx_LIB_DIRS = $(gfx_HOME)/lib

gfx_INCLUDE_DIRS = -I. -I$(gfx_HOME)/include/allegro5

gfx_HEADERS = peripherals.h
gfx_SOURCES = peripherals.cc

gfx_LIBS = -lallegro -lallegro_main -lallegro_primitives.5.2 -lstdc++

gfx_EXEC = gfx

gfx_DEPENDANCIES = \
	Makefile \
	$(gfx_HEADERS) \
	$(gfx_SOURCES)

gfx: $(gfxL_DEPENDANCIES)
	gcc -g -o $@ $(gfx_SOURCES) $(gfx_INCLUDE_DIRS) -L$(gfx_LIB_DIRS) $(gfx_LIBS)
	./$(gfx_EXEC)