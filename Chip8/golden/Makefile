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