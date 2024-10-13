SRC := src
OBJ := obj
BIN := bin
EXECUTABLE := aes

# Source files (assuming main.cpp and .hpp files are in src)
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))

# Include directories (for the .h and .hpp files)
INCS := -Iinclude/ -Isrc/

# Compiler and flags
CC := g++
CFLAGS := -g -Wall -std=c++17 $(INCS)

# Target for the executable
all: $(OBJ) $(BIN) $(EXECUTABLE)

# Create Directories
$(OBJ) $(BIN):
	mkdir -p $@

# Link object files to create the final executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)/$(EXECUTABLE)

# Rule to compile .cpp files into .o files
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: all
	./$(BIN)/$(EXECUTABLE)

# Clean the compiled files
clean:
	rm -rf $(OBJ)/*.o $(BIN)/$(EXECUTABLE)

.PHONY: all run clean
