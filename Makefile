# Makefile to build the project

# Parameters
CC = g++
CFLAGS = -Wall -O2
LIBS = -lpapi

SRC = src/
BIN = bin/
OUTPUT = output/22_02/results_cpp.csv


# Targets
.PHONY: all clean
all: $(BIN)/matrix

$(BIN)/matrix: $(SRC)/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: run_mult_standard
run_mult_standard: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		./$(BIN)/matrix 1 $$size $(OUTPUT); \
	done

.PHONY: run_mult_line
run_mult_line: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		./$(BIN)/matrix 2 $$size $(OUTPUT); \
	done 
	@for size in `seq 4096 2048 10240`; do \
		./$(BIN)/matrix 2 $$size $(OUTPUT); \
	done

.PHONY: run_mult_block
run_mult_block: $(BIN)/matrix
	@for size in `seq 4096 2048 10240`; do \
		for block in 128 256 512; do \
			./$(BIN)/matrix 3 $$size $(OUTPUT) $$block; \
		done \
	done

.PHONY: run
run: run_mult_standard run_mult_line run_mult_block

.PHONY: clean
clean:
	rm -f $(BIN)/matrix
