#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// functie ce verifica daca un fisier exista
int fileExists(const char *path) {
	return access(path, F_OK) != -1;
}

// creaza un director in cazul in care nu exista (pt directorul "./files")
void initFilesDirectory(const char *dirPath) {
	if (!fileExists(dirPath)) {
		if (-1 == mkdir(dirPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) {
			exit(1);
		}
	}
}

bool stringIsInteger(const char *str) {
    if (strlen(str) > 5) {
        return false;
    }
    int num = 0;
    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        ++str;
    }
    if (*str) {
        return false;
    }
    return num;
}

int readIntFromConsole(const char *prompt) {
	while (true) {
		if (prompt) {
			std::cout << prompt;
		}
		std::string line;
		std::getline(std::cin, line);
		if (!stringIsInteger(line.c_str()) || atoi(line.c_str()) >= (1 << 16)) {
			std::cout << "Expected an integer less than 1 << 16!\n";
			continue;
		}
		return atoi(line.c_str());
	}
	return 0;
}

// valideaza argumentele primite la consola
int validateArgs(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid arguments!\nUsage: %s port\n", argv[0]);
        exit(1);
    }
    int port;
    while (true) {
        if (!stringIsInteger(argv[1])) {
            break;
        }
        port = atoi(argv[1]);
        if (port >= (1 << 16)) {
            break;
        }
        return port;
    }
    printf("Invalid second argument! Expected an integer less than 1 << 16!\n");
    exit(1);
}

// construieste un buffer ce are forma: primii 4 bytes lungimea bufferului, iar apoi len bytes
bool buildBufferPrefixWithLen(int len, const byte buffer[], int targetBufferSize, byte targetBuffer[]) {
	if (len + sizeof(len) > targetBufferSize) {
		printf("Invalid arguments for a buildBufferPrefixWithLen() call! The targetBuffer is not large enough!\n");
		return false;
	}
	int* intPtr = (int*)targetBuffer;
	*intPtr = len;
	int add = sizeof(len);
	for (int i=0;i<len;++i) {
		targetBuffer[add+i] = buffer[i];
	}
	return true;
}

#endif // COMMON_FUNCTIONS_H
