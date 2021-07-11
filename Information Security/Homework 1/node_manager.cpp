/*
Documentation:
- UDP Server:
    https://cppsecrets.com/users/41129711010797106994610011511264103109971051084699111109/UDP-Server-Program-in-C.php
    https://www.geeksforgeeks.org/udp-server-client-implementation-c/
- Random generators:
	https://wiki.openssl.org/index.php/Random_Numbers
	https://www.openssl.org/docs/man1.0.2/man3/RAND_bytes.html
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

#include <openssl/rand.h>

#include "flags.h"
#include "common_functions.h"
#include "crypto.h"

byte buffer[BUFFER_SIZE]; // buffer in care va fi criptata cheia generata

void sendKey(int sockfd, const struct sockaddr* clientAddr, unsigned int clientLen);

int main(int argc, char* argv[]) {
    int serverPort = validateArgs(argc, argv);
    int sockfd;
    struct sockaddr_in serverAddr; // server's address
    struct sockaddr_in clientAddr; // client's address
    struct hostent* hostp; // information about the client of the server
    unsigned int clientLen = sizeof(clientAddr);
    int optval = 1; // SO_REUSEADDR
    char *hostaddrp; // dotted decimal host addr string

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Failed to create the socket!\n");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) < 0) {
        printf("Failed to set the option \"SO_REUSEADDR\" to socket!\n");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((unsigned short)serverPort);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error when calling \"bind()\"!\n");
        exit(1);
    }

	printf("Waiting for requests on port %d!\n", serverPort);

    RAND_poll(); // random seed for the bytes generator
    
    bool running = true;
    while (running) {
        memset(&clientAddr, 0, clientLen);
        byte requestOption = 0;
        if (recvfrom(sockfd, &requestOption, sizeof(requestOption), 0,
            (struct sockaddr*)&clientAddr, &clientLen) < 0) {
            printf("Error when reading the option flag!\n");
            exit(1);
        }

        hostp = gethostbyaddr((const char*)&clientAddr.sin_addr.s_addr,
            sizeof(clientAddr.sin_addr.s_addr), AF_INET);
        
        if (!hostp) {
            printf("Error when calling \"gethostbyaddr()\"!\n");
            exit(1);
        }
        hostaddrp = inet_ntoa(clientAddr.sin_addr);
        if (!hostaddrp) {
            printf("Error when calling \"inet_ntoa()\"!\n");
            exit(1);
        }

		const char* requestType;
        switch(requestOption) {
            case KEY_MANAGER_GET_KEY:
            	sendKey(sockfd, (const struct sockaddr*)&clientAddr, clientLen);
                // trimite o cheie random peer-ului care a trimis request-ului
            	requestType = "KEY_MANAGER_GET_KEY";
                break;
            default:
                requestType = "INVALID";
                break;
        }
        printf("Received \"%s\" request from: %s (%s)!\n", requestType, hostp->h_name, hostaddrp);
    }

    return 0;
}

void sendKey(int sockfd, const struct sockaddr* clientAddr, unsigned int clientLen) {
	byte key[keySize]; // buffer in care va fi generata o cheie
	byte encryptedKey[keySize]; // buffer in care va fi pusa cheia criptata
	int encryptedKeyLen = 0;
	
	int rc = RAND_bytes(key, keySize); // generez keySize bytes (128 biti)
	if (rc != 1) {
		encryptedKeyLen = -1;
		printf("Error when generating key!\n");
	}
	
	printf("Key: ");
	for (int i=0;i<keySize;++i) {
		printf("%d ", int(key[i]));
	}
	printf("\n");
	
	if (encryptedKeyLen != -1) {
		if (!encrypt(key, keySize, k3, iv, encryptedKey, encryptedKeyLen)) { // criptez cheia 
			encryptedKeyLen = -1;
		} else {
			if (!buildBufferPrefixWithLen(encryptedKeyLen, encryptedKey, BUFFER_SIZE, buffer)) {
                // construiesc blockul trimis nodului care a trimis cererea
				encryptedKeyLen = -1;
			}
		}	
	}
	
	if (encryptedKeyLen == -1) {
        // trimit doar un -1, indicand eroare
		if (sendto(sockfd, &encryptedKeyLen, sizeof(encryptedKeyLen), 0, clientAddr, clientLen) < 0) {
			printf("Error when sending a flag that indicates a server error to a client!\n");
		}
	} else {
        // trimit un buffer ce contine keyLen+key
		if (sendto(sockfd, buffer, encryptedKeyLen + sizeof(encryptedKeyLen), 0, clientAddr, clientLen) < 0) {
			printf("Error when sending a newly generated key2 to a client!\n");
		}
	}
}
