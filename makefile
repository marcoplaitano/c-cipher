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


.PHONY: ccipher dirs clean


# Compile and create main executable, same as default target.
ccipher: $(EXEC)

# Create needed directories if they do not already exist.
dirs:
	$(shell if [ ! -d $(BIN_DIR) ]; then mkdir -p $(BIN_DIR); fi)
	$(shell if [ ! -d $(BUILD_DIR) ]; then mkdir -p $(BUILD_DIR); fi)


# Delete object files and executables.
clean:
	-rm $(BUILD_DIR)/* $(BIN_DIR)/*
