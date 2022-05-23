/**
 * @file util.c
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * CCIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 *
 * Copyright 2022 Marco Plaitano
 */

#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void secure_free(void *ptr) {
    free(ptr);
    ptr = NULL;
}


int error(const char *str, ...) {
    fprintf(stderr, "ccipher ERROR: ");
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    va_end(args);
    fprintf(stderr, "\n");
    return 1;
}
