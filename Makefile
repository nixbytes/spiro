CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc
LIBS = -lm

SRC_DIR = src
OBJS_DIR = objs
TARGET = spiro

# Find all .c files in the src directory
SRCS = $(shell find $(SRC_DIR) -name "*.c")
# Map .c files to .o files in the objs directory, maintaining subfolder structure
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile .c to .o, creating directories as needed
$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJS_DIR) $(TARGET)
