/**
 * @file main.c
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * C-CIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"

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
    if (argc < NUM_ARGS) {
        fprintf(stderr, "Wrong number of arguments; check the README file.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(IN_FILE, OUT_FILE) == 0) {
        fprintf(stderr, "Input File and Output File must be different.\n");
        return EXIT_FAILURE;
    }

    if (strlen(PASSWORD) < 1) {
        fprintf(stderr, "Password must be a non-empty string.\n");
        return EXIT_FAILURE;
    }

    cipher_mode mode;
    if (strcmp(MODE, "encrypt") == 0)
        mode = MODE_ENCRYPT;
    else if (strcmp(MODE, "decrypt") == 0)
        mode = MODE_DECRYPT;
    else {
        fprintf(stderr, "Mode '%s' is unrecognized.\n", MODE);
        return EXIT_FAILURE;
    }

    if (cipher(PASSWORD, IN_FILE, OUT_FILE, mode)) {
        fprintf(stderr, "%sion failed.\n", MODE);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
