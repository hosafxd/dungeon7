# Makefile for Dungeon7

# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directories
SRCDIR = .
OBJDIR = .
BINDIR = .

# Source and Object Files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/ROOM.c $(SRCDIR)/ITEM.c $(SRCDIR)/MONSTER.c
OBJS = $(OBJDIR)/main.o $(OBJDIR)/ROOM.o $(OBJDIR)/ITEM.o $(OBJDIR)/MONSTER.o

# Executable
TARGET = $(BINDIR)/Dungeon7.exe

# Default Rule
all: $(TARGET)

# Linking the Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling Object Files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
