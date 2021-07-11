/*
Documentation:
https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
*/
#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <string>
#include <cstdlib>

#include "flags.h"

#define byte unsigned char

// functie ce va cripta plaintext-ul indicat de "plainText" (de lungime plainTextLen)
bool encrypt(const byte* plainText, int plainTextLen, const byte* key, const byte* iv, byte *cipherText, int& cipherTextLen);

// functie ce va decripta ciphertext-ul indicat de "cipherText" (de lungime cipherTextLen)
bool decrypt(const byte* cipherText, int cipherTextLen, const byte* key, const byte* iv, byte *plainText, int& plainTextLen);

#endif // CRYPTO_H
