# syntax of every block in a makefile
# target: prerequisites
# 		command

# automatic variables:
# $@ - represents the name of the current target
# $< - represents the first prerequisite
# $^ - represents all prerequisites listed for that target

# compiler and flags
# -Iinclude flag tells the compiler to search inside the include/ directory automatically whenever it reads a #include "header.h" line in the c code
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# directories and target name
SRC_DIR = src
TARGET = main

# find all .c files in src/ and convert them to .o object file paths
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# default target. here, to complete 'all', run the 'main' rule first
all: $(TARGET)

# link target executable
$(TARGET): $(OBJS) 
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# compile individual .c files into .o files
# $< references the first prerequisite (the .c file)
# $@ references the target file (.o file or the final executable)
%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

# rule to clean up the compilation assets
.PHONY: clean
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)