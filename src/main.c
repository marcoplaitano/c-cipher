/**
 * @file main.c
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * CCIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 *
 * Copyright 2022 Marco Plaitano
 */

#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "util.h"

/** Number of command line arguments needed. 1st one is the program itself. */
#define NUM_ARGS 1 + 4

/** The input file to encrypt/decrypt. */
#define IN_FILE argv[1]
/** The output file in which to write the result. */
#define OUT_FILE argv[2]
/** The mode in which to apply the cipher: either 'encrypt' or 'decrypt'. */
#define MODE argv[3]
/** Password to use as source for the cipher's key. */
#define PASSWORD argv[4]


int main(int argc, char **argv) {
    if (argc < NUM_ARGS)
        return error("Wrong number of arguments; check the README file.");

    if (strcmp(IN_FILE, OUT_FILE) == 0)
        return error("Input File and Output File must be different.");

    if (strlen(PASSWORD) < 1)
        return error("Password must be a non-empty string.");

    cipher_mode_e mode;
    if (strcmp(MODE, "encrypt") == 0)
        mode = MODE_ENCRYPT;
    else if (strcmp(MODE, "decrypt") == 0)
        mode = MODE_DECRYPT;
    else
        return error("Mode '%s' is unrecognized.", MODE);

    if (cipher(PASSWORD, IN_FILE, OUT_FILE, mode))
        return error("%sion failed.", MODE);

    return EXIT_SUCCESS;
}
