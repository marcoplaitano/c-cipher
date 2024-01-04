/**
 * @file cipher.c
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * C-CIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 */

#include "cipher.h"

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#define BUFF_LEN 16


/**
 * @brief Free a pointer and set it to NULL.
 * @param ptr: Pointer to free.
 */
static void secure_free(void *ptr) {
    free(ptr);
    ptr = NULL;
}



/**
 * @brief Fill the IV with the correct bytes, based on whether it will be used
 *        for encryption or decryption.
 *
 * The following statements assume the bits are in big-endian order.
 * The IV (Initialization Vector) is a string of `n = 16` bytes. `n` is also the
 * cipher's block length.
 * The last (LSB) 8 bytes contain a counter which will be incremented at every
 * new encrypted block.
 * The first (MSB) 8 bytes contain the "nonce" (a random sequence of bits).
 * When encrypting, the nonce is filled with random bytes; these will be
 * prepended to the ciphertext (in the output file) in order to read them during
 * the decryption procedure.
 *
 * @param IV: The Initialization Vector.
 * @param mode: Operating mode. see #cipher_mode_e
 * @param in_file: Input file.
 * @param out_file: Output file.
 * @return Integer value; either `0` (success) or `1` (failure).
 */
static int IV_fill(unsigned char *IV, cipher_mode mode, FILE *in_file,
                   FILE *out_file)
{
    /* Reset the last 8 bytes. */
    memset(IV + 8, 0, 8);
    if (*((int *)IV + 8) != 0 || *((int *)(IV + 12)) != 0) {
        fprintf(stderr, "Could not reset first 8 bytes of IV.\n");
        return 1;
    }

    if (mode == MODE_ENCRYPT) {
        /* Fill first 8 bytes with randomness and write them onto the file. */
        if (!RAND_bytes(IV, 8)) {
            fprintf(stderr, "Could not generate random bytes for IV.\n");
            return 1;
        }
        if (fwrite(IV, 1, 8, out_file) < 8) {
            fprintf(stderr, "Could not write 8 bytes to file.\n");
            return 1;
        }
    }
    else if (mode == MODE_DECRYPT) {
        /* Read first 8 bytes from the file. */
        if (fread(IV, 1, 8, in_file) < 8) {
            fprintf(stderr, "Could not read 8 bytes from file.\n");
            return 1;
        }
    }

    return 0;
}



int cipher(const char *password, const char *in_file_path,
           const char *out_file_path, cipher_mode mode)
{
    if (strcmp(in_file_path, out_file_path) == 0) {
        fprintf(stderr, "Input File and Output File must be different.\n");
        return 1;
    }

    /* Return value. */
    int ret = 1;

    /* Allocate needed resources. */
    unsigned char *key = malloc(32);
    if (key == NULL) {
        fprintf(stderr, "Could not allocate 32 bytes of memory for the key.\n");
        goto error_alloc_key;
    }
    unsigned char *IV = malloc(16);
    if (IV == NULL) {
        fprintf(stderr, "Could not allocate 16 bytes of memory for the IV.\n");
        goto error_alloc_IV;
    }
    unsigned char *in_buff = malloc(BUFF_LEN);
    if (in_buff == NULL) {
        fprintf(stderr, "Could not allocate %d bytes for the input buffer.\n", BUFF_LEN);
        goto error_alloc_in_buff;
    }
    unsigned char *out_buff = malloc(BUFF_LEN);
    if (out_buff == NULL) {
        fprintf(stderr, "Could not allocate %d bytes for the output buffer.\n", BUFF_LEN);
        goto error_alloc_out_buff;
    }

    /* Open file streams. */
    FILE *in_file = fopen(in_file_path, "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Could not open file '%s'.\n", in_file_path);
        goto error_open_in_file;
    }
    FILE *out_file = fopen(out_file_path, "wb");
    if (out_file == NULL) {
        fprintf(stderr, "Could not open file '%s'.\n", out_file_path);
        goto error_open_out_file;
    }

    /* Create instance of AES-256-CTR cipher. */
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        fprintf(stderr, "Could not create instance of Cipher CTX.\n");
        goto error_alloc_ctx;
    }
    EVP_CIPHER *cipher = EVP_CIPHER_fetch(NULL, "AES-256-CTR", NULL);
    if (cipher == NULL) {
        fprintf(stderr, "Could not create instance of Cipher.\n");
        goto error_alloc_cipher;
    }

    /* Create the Encryption Key by Hashing the user's password. */
    SHA256((unsigned char *)password, strlen(password), key);

    /* Fill the IV with the correct bytes. */
    if (IV_fill(IV, mode, in_file, out_file)) {
        goto error;
    }

    if (!EVP_EncryptInit_ex2(ctx, cipher, key, IV, NULL)) {
        fprintf(stderr, "Error in EVP_EncryptInit_ex2.\n");
        goto error;
    }

    /*
     * Read BUFF_LEN bytes from the input file, apply the cipher on them (at
     * this point it matters not whether it is encryption or decryption) and
     * save the result on the output file.
     * Repeat the process until the entire file has been processed.
     */
    int out_len = 0;
    while (1) {
        int num_read = fread(in_buff, 1, BUFF_LEN, in_file);
        /* Reached EOF. */
        if (num_read <= 0)
            break;
        if (!EVP_EncryptUpdate(ctx, out_buff, &out_len, in_buff, num_read)) {
            fprintf(stderr, "Error in EVP_EncryptUpdate.\n");
            goto error;
        }
        if (fwrite(out_buff, 1, out_len, out_file) < out_len) {
            fprintf(stderr, "Could not write %d bytes to file '%s'.\n",
                            out_len, out_file_path);
            goto error;
        }
    }
    if (!EVP_EncryptFinal_ex(ctx, out_buff, &out_len)) {
        fprintf(stderr, "Error in EVP_EncryptFinal_ex.\n");
        goto error;
    }
    if (fwrite(out_buff, 1, out_len, out_file) < out_len) {
        fprintf(stderr, "Could not write %d bytes to file '%s'.\n",
                        out_len, out_file_path);
        goto error;
    }

    /* Every operation has been successful. */
    ret = 0;

    /* Free resources and close file streams. */
error:
    EVP_CIPHER_free(cipher);
error_alloc_cipher:
    EVP_CIPHER_CTX_free(ctx);
error_alloc_ctx:
    fclose(out_file);
error_open_out_file:
    fclose(in_file);
error_open_in_file:
    secure_free(out_buff);
error_alloc_out_buff:
    secure_free(in_buff);
error_alloc_in_buff:
    secure_free(IV);
error_alloc_IV:
    secure_free(key);
error_alloc_key:
    return ret;
}
