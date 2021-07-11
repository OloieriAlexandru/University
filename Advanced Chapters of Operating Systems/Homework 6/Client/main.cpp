#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <iostream>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

/*
Documentation:
https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-sendto
*/

const int PORT = 4444;
const char* SERVER_ADDRESS = "127.0.0.1";
const char* downloadFileName = "D:\\FII\\CSSO6Dir\\downloaded_file.txt";

const int MAX_RESPONSE_LEN = (1 << 16);
char RESPONSE[MAX_RESPONSE_LEN + 1];

HANDLE createFile(const char* fileName) {
	return CreateFile(
		fileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		INVALID_HANDLE_VALUE);
}

void init() {
	int iResult = 0;
	WSADATA wsaData;

	//-----------------------------------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed with error %d\n", iResult);
		exit(1);
	}
}

bool startsWith(const char* source, const char* target) {
	while (*source && *target) {
		if (*source != *target) {
			return false;
		}
		++source;
		++target;
	}
	if (!*source) {
		return false;
	}
	return true;
}

void saveToFile(const char* fileContent) {
	HANDLE fHandle = createFile(downloadFileName);

	if (!WriteFile(
		fHandle,
		fileContent,
		strlen(fileContent),
		NULL, NULL)) {
		printf("Failed to write the downloaded content to file \"%s\"!", downloadFileName);
	}

	CloseHandle(fHandle);
}

void initClient(SOCKET& sd, struct sockaddr_in& RecvAddr) {
	//-----------------------------------------------
	// Create a socket for sending data
	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sd == INVALID_SOCKET) {
		printf("socket failed with error %d\n", WSAGetLastError());
		exit(1);
	}

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	RecvAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
}

void close(SOCKET sd) {
	int iResult;

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	iResult = closesocket(sd);
	if (iResult == SOCKET_ERROR) {
		printf("closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}
	WSACleanup();
}

std::string END_COMMAND = "quit";

int main() {
	init();

	SOCKET sd;
	struct sockaddr_in RecvAddr;
	initClient(sd, RecvAddr);

	struct sockaddr_in ServerAddr;
	int serverAddrLen = sizeof(ServerAddr);

	bool running = true;
	while (running) {
		printf("------\n");
		printf("olo>");
		std::string line;
		std::getline(std::cin, line);

		if (line == END_COMMAND) {
			break;
		}

		int iResult = sendto(sd, line.c_str(), 
			line.size(), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
		if (iResult == SOCKET_ERROR) {
			printf("Failed to send the command to the server! Try again..\n");
			continue;
		}
		// printf("Command send successfully!\n");

		int responseLen = recvfrom(sd, RESPONSE, MAX_RESPONSE_LEN, 0, (SOCKADDR*)&ServerAddr, &serverAddrLen);
		if (responseLen == SOCKET_ERROR) {
			printf("No response received!\n");
			continue;
		}
		if (responseLen <= MAX_RESPONSE_LEN) {
			RESPONSE[responseLen] = '\0';
		}
		if (startsWith(line.c_str(), "download ")) {
			saveToFile(RESPONSE);
			printf("The downloaded content was saved to file: \"%s\"!", downloadFileName);
		} else {
			printf("%s\n", RESPONSE);
		}
	}

	close(sd);
	return 0;
}
