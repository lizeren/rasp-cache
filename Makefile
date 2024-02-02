# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-O3 

# Define the source file(s)
SRC=cache.c
#SRC=spectre.c

# Define the target executable
TARGET=execute

# ANSI color codes
GREEN=\033[0;32m
BLUE=\033[0;34m
NC=\033[0m # No Color

# Default target: build the executable
all: $(TARGET)

$(TARGET): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@printf "$(GREEN)Compilation complete$(NC)\n"

# Execute target: execute the compiled program
run: $(TARGET)
	@./$(TARGET)
	@printf "$(BLUE)Execution complete$(NC)\n"

# Clean target: remove the executable
clean:
	@rm -f $(TARGET)
	@printf "$(GREEN)Cleaned$(NC)\n"

