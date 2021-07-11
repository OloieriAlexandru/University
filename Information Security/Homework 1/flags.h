#ifndef FLAGS_H
#define FLAGS_H

#define byte unsigned char

// Flags used by normal nodes when sending a request to the node manager
const byte KEY_MANAGER_GET_KEY = 2; // get a random encrypted key
const byte KEY_MANAGER_CLOSE = 2; // close the key manager

const int FLAG_FILE_SUCCESS = -1;
const int FLAG_FILE_FAIL = -2;

const byte FLAG_ECB = 10;
const byte FLAG_CFB = 11;

const byte FLAG_START = 111;

const int BUFFER_SIZE = 1024;

const int BLOCK_SIZE = 128;
const int BLOCK_SIZE_BYTES = 128 / 8;

const int ivSize = 16;
const int keySize = 16; 

const byte iv[ivSize] = { 117, 152, 164, 104, 253, 218, 106, 191, 116, 108, 130, 159, 166, 253, 203, 169 };
const byte k3[keySize] = { 72, 229, 205, 76, 179, 25, 75, 234, 118, 45, 135, 22, 171, 153, 2, 163 };

#endif // FLAGS_H
