# Makefile to build the project

# Parameters
CC = g++
CFLAGS = -Wall -O2
LIBS = -lpapi

SRC = src/
BIN = bin/
OUTPUT = output/results.csv


# Targets
.PHONY: all clean
all: $(BIN)/matrix

$(BIN)/matrix: $(SRC)/*.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: run
run: $(BIN)/matrix
	@for size in `seq 600 400 3000`; do \
		./$(BIN)/matrix 1 $$size $(OUTPUT); \
	done


.PHONY: clean
clean:
	rm -f $(BIN)/matrix