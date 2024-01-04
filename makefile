# File:   makefile
# Author: Marco Plaitano
# Date:   22 May 2022
#
# C-CIPHER
# A program to encrypt/decrypt a file using the AES-CTR-256 cipher.

BIN_DIR := bin
BUILD_DIR := build
INCLUDE_DIR := include
SRC_DIR := src

CC := gcc
CFLAGS = -g -I$(INCLUDE_DIR)/
CLIBS = -lcrypto -lssl

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
EXEC := bin/ccipher


# Default target: create main executable.
$(EXEC): dirs $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(CLIBS) -o $@

# Create object files.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: ccipher install dirs clean


# Compile and create main executable, same as default target.
ccipher: $(EXEC)

# Compile, then copy executable to /usr/local/bin and manpage to /usr/local/man
install: ccipher
	cp $(EXEC) /usr/local/bin/ccipher
	chmod 755 /usr/local/bin/ccipher
	mkdir -p /usr/local/man/man1
	cp ccipher.1 /usr/local/man/man1/
	chmod 644 /usr/local/man/man1/ccipher.1

# Create needed directories if they do not already exist.
dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

# Delete object files and executables.
clean:
	-rm $(BUILD_DIR)/* $(BIN_DIR)/*
