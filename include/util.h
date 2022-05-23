/**
 * @file util.h
 * @author Marco Plaitano
 * @date 22 May 2022
 *
 * CCIPHER
 * A program to encrypt/decrypt a file using the AES-CTR-256 cipher.
 *
 * Copyright 2022 Marco Plaitano
 */

#ifndef UTIL_H
#define UTIL_H


/**
 * @brief Free a pointer and set it to NULL.
 * @param ptr: Pointer to free.
 */
void secure_free(void *ptr);

/**
 * @brief Display formatted error message.
 *
 * It takes a variable number of arguments in input in order to print the
 * formatted version of the message string (like a call to printf).
 * A new-line is added at the end.
 *
 * @param str: Message string.
 * @return 1. To be interpreted as an exit code, it can be used as shortcut to
 *         exit a function.
 */
int error(const char *str, ...);


#endif /* UTIL_H */
