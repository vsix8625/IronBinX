CC = gcc

CFLAGS = -std=gnu17 -Wall -Werror -O2 -Iinclude

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = build/objects

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Executable name
EXECUTABLE = $(BIN_DIR)/IronbinX

all: $(EXECUTABLE)

# Link object files to create executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run:
	$(EXECUTABLE)

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)

.PHONY: all run clean
