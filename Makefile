# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wno-unused-variable -fsanitize=address -g

# Target executable
TARGET = number-server

# Source files
SOURCES = number-server.c http-server.c

# Build target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Clean target to remove the compiled files
clean:
	rm -f $(TARGET)

