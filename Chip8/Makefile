LIB_DIRS=$(SYSTEMC_HOME)/lib-macosx64

INCLUDE_DIRS = -I. -I$(SYSTEMC_HOME)/include

HEADERS = fir.h tb.h
SOURCES = main.cc fir.cc tb.cc

DEPENDANCIES = \
	Makefile \
	$(HEADERS) \
	$(SOURCES)

LIBS= -lsystemc -lstdc++ -lm

TESTS= main

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
