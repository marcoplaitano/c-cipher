/**
 * @file cipher.h
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * C-CIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 */

#ifndef CIPHER_H
#define CIPHER_H


/** @brief List of operative modes for the cipher. */
typedef enum {
    MODE_ENCRYPT,
    MODE_DECRYPT
} cipher_mode;


/**
 * @brief Apply a cipher to the contents of a file and write the output in a
 *        separate file.
 *
 * The cipher used is AES-CTR-256.
 *
 * @param password: Password string used as source for the encryption key.
 * @param in_file_path: Path to the input file.
 * @param out_file_path: Path to the output file. Must be different from
 *                       input_file.
 * @param mode: Operative mode for the cipher; see #cipher_mode.
 * @return Integer value; either `0` (success) or `1` (failure).
 */
int cipher(const char *password, const char *in_file_path,
           const char *out_file_path, cipher_mode mode);


#endif /* CIPHER_H */
