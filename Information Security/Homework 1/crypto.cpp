#include "crypto.h"

bool encrypt(const byte* plainText, int plainTextLen, const byte* key, const byte* iv, byte *cipherText, int& cipherTextLen) {
	if (plainTextLen != BLOCK_SIZE_BYTES) {
		printf("The encryption function should only encrypt an AES block (128 bits)! %d passed!\n", plainTextLen);
		exit(1);
	}

	EVP_CIPHER_CTX *ctx;
	int len;
	
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		printf("Failed to initialize an EVP_CIPHER_CTX!\n");
		return false;
	}
	
	int initResult = EVP_EncryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, key, iv);

	if (initResult != 1) {
		printf("Failed to initialize the encryption cipher!\n");
		return false;
	}
	
	if (1 != EVP_EncryptUpdate(ctx, cipherText, &len, plainText, plainTextLen)) {
		printf("Error when calling EVP_EncryptUpdate()!\n");
		return false;
	}
	cipherTextLen = len;
	
	if (1 != EVP_EncryptFinal_ex(ctx, cipherText + len, &len)) {
		printf("Error when calling EVP_EncryptFinal_ex()!\n");
		return false;
	}
	cipherTextLen += len;
		
	EVP_CIPHER_CTX_free(ctx);
	return true;
}

bool decrypt(const byte* cipherText, int cipherTextLen, const byte* key, const byte* iv, byte *plainText, int& plainTextLen) {
	if (cipherTextLen != BLOCK_SIZE_BYTES) {
		printf("The decryption function should only decrypt an AES block (128 bits)! %d passed!\n", cipherTextLen);
		exit(1);
	}
    
	EVP_CIPHER_CTX *ctx;
    int len;
    
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		printf("Failed to initialize an EVP_CIPHER_CTX!\n");
		return false;
	}
	
	int initResult = EVP_DecryptInit_ex(ctx, EVP_aes_128_cfb(), NULL, key, iv);

	if (initResult != 1) {
		printf("Failed to initialize the decryption cipher!\n");
		return false;
	}
	
	if (1 != EVP_DecryptUpdate(ctx, plainText, &len, cipherText, cipherTextLen)) {
		printf("Error when calling EVP_DecryptUpdate()!\n");
		return false;
	}
	plainTextLen = len;
	
	if (1 != EVP_DecryptFinal_ex(ctx, plainText + len, &len)) {
		printf("Error when calling EVP_DecryptFinal_ex()!\n");
		return false;
	}
	plainTextLen += len;
		
	EVP_CIPHER_CTX_free(ctx);
	return true;
}
