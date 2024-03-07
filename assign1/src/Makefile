# Makefile to build the project

# Parameters
CC = g++
CFLAGS = -Wall -O2 -fopenmp
LIBS = -lpapi

SRC = functions/
BIN = bin/
# You need check the path
OUTPUT = output/07_03/results_cpp_test2.csv


# Targets
.PHONY: all clean
all: $(BIN)/matrix

$(BIN)/matrix: $(SRC)/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Part 1 Task 1
.PHONY: run_mult_standard
run_mult_standard: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		./$(BIN)/matrix 1 $$size $(OUTPUT); \
	done

# Part 1 Task 2
.PHONY: run_mult_line
run_mult_line: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		./$(BIN)/matrix 2 $$size $(OUTPUT); \
	done 
	@for size in `seq 4096 2048 10240`; do \
		./$(BIN)/matrix 2 $$size $(OUTPUT); \
	done

# Part 1 Task 3
.PHONY: run_mult_block
run_mult_block: $(BIN)/matrix
	@for size in `seq 4096 2048 10240`; do \
		for block in 128 256 512; do \
			./$(BIN)/matrix 3 $$size $(OUTPUT) $$block; \
		done \
	done

# Part 2 Task 1
.PHONY: run_mult_line_parallel_v1
run_mult_line_parallel_v1: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		for n_threads in 4 6 8; do \
			./$(BIN)/matrix 4 $$size $(OUTPUT) $$n_threads; \
		done \
	done 
	@for size in `seq 4096 2048 10240`; do \
		for n_threads in 4 6 8; do \
			./$(BIN)/matrix 4 $$size $(OUTPUT) $$n_threads; \
		done \
	done

# Part 2 Task 2
.PHONY: run_mult_line_parallel_v2
run_mult_line_parallel_v2: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		for n_threads in 4 6 8; do \
			./$(BIN)/matrix 5 $$size $(OUTPUT) $$n_threads; \
		done \
	done 
	@for size in `seq 4096 2048 10240`; do \
		for n_threads in 4 6 8; do \
			./$(BIN)/matrix 5 $$size $(OUTPUT) $$n_threads; \
		done \
	done

# Part 2 Experimental block algorithm parallel
.PHONY: run_mult_block_parallel
run_mult_block_parallel: $(BIN)/matrix
	@for size in `seq 4096 2048 10240`; do \
		for n_threads in 4 6 8; do \
			./$(BIN)/matrix 6 $$size $(OUTPUT) 256 $$n_threads; \
		done \
	done


.PHONY: run
run: run_mult_standard run_mult_line run_mult_block run_mult_line_parallel_v1 run_mult_line_parallel_v2 run_mult_block_parallel

.PHONY: clean
clean:
	rm -f $(BIN)/matrix
