# Compiler and Flags

PARENT_DIR := $(shell basename $(dir $(realpath .)))

CC = gcc

CFLAGS = -std=gnu17 -Wall -Werror -O1 -I../inc

# Example lib flags
# LDFLAGS = -L/usr/local/lib

# Example libso flags
# LIBS = -lSDL3 -lSDL3_ttf -lACE

# Directories
SRC_DIR = ../src
OBJ_DIR = objects
BIN_DIR = ../bin

# Source and Object Files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BIN_DIR)/$(PARENT_DIR)

# Default Target
all: $(EXECUTABLE)

# Linking
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)         # Create bin directory if it doesn't exist
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

	@mkdir -p ../config
	@echo bin/$(PARENT_DIR) > ../config/app.conf


# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)         # Create objects directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* ../config/app.conf

.PHONY: all clean
