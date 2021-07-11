#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

#define CLOSE_HANDLES \
	WaitForSingleObject(processInformation.hProcess, INFINITE); \
	if (mutexSynchronization) { \
		CloseHandle(hEventSentPair); \
		CloseHandle(hEventReceivedPair); \
	} else { \
		CloseHandle(hMtx); } \
	CloseHandle(processInformation.hProcess); \
	CloseHandle(processInformation.hThread); \
	CloseHandle(hMemoryFile); \

bool whiteCharacter(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\n';
}

void trim(std::string& str) {
	while (str.size() && whiteCharacter(str.back())) {
		str.pop_back();
	}
	std::reverse(str.begin(), str.end());
	while (str.size() && whiteCharacter(str.back())) {
		str.pop_back();
	}
	std::reverse(str.begin(), str.end());
}

int parseStringInt(const std::string& str) {
	unsigned int value = 0;
	if (str.size() > 9) {
		return -1;
	}
	for (auto digit : str) {
		if (!(digit >= '0' && digit <= '9')) {
			return -1;
		}
		value = value * 10 + (digit - '0');
	}
	return value;
}

bool getOptionIsMutexSynchronization() {
	while (true) {
		std::cout << "What type of synchronization do you want? 1 - mutex, 2 - events: ";
		std::string line;
		std::getline(std::cin, line);
		trim(line);
		int intValue = parseStringInt(line);
		if (intValue == -1 || !(intValue == 1 || intValue == 2)) {
			std::cout << "Invalid option!\n";
			continue;
		}
		if (intValue == 1) {
			return true;
		}
		return false;
	}
}

const char* event1Name = "sent-pair";
const char* event2Name = "received-pair";
const char* mutexName = "mtx";

const char* memoryMappedFileName = "processes_info.info";
const char* program2ExePath = "D:\\github\\Uni\\CSSO\\Homework 3\\Homework 3\\Debug\\Program 2.exe";
const DWORD STOP_FLAG = 10000000;

void enterCriticalSectionEvents(HANDLE, HANDLE hEventReceivedPair) {
	WaitForSingleObject(hEventReceivedPair, INFINITE);
	ResetEvent(hEventReceivedPair);
}

void enterCriticalSectionMutex(HANDLE hMtx) {
	WaitForSingleObject(hMtx, INFINITE);
}

void exitCriticalSectionEvents(HANDLE hEventSentPair, HANDLE) {
	SetEvent(hEventSentPair);
}

void exitCriticalSectionMutex(HANDLE hMtx) {
	ReleaseMutex(hMtx);
}

int main() {
	HANDLE hEventSentPair = NULL;
	HANDLE hEventReceivedPair = NULL;
	HANDLE hMtx = NULL;

	PROCESS_INFORMATION processInformation;
	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	srand((unsigned int)time(NULL));
	
	bool mutexSynchronization = getOptionIsMutexSynchronization();

	HANDLE hMemoryFile = CreateFileMapping(
		NULL,
		NULL,
		PAGE_READWRITE,
		0,
		1 << 16,
		memoryMappedFileName);

	if (hMemoryFile == NULL) {
		printf("Failed to create the \"%s\" memory-mapped file!\n", memoryMappedFileName);
		return 1;
	}

	auto buffer = (char*)MapViewOfFile(
		hMemoryFile,
		FILE_MAP_WRITE,
		0, 0, 0);

	if (buffer == NULL) {
		printf("Failed to map view of \"%s\" memory-mapped file!\n", memoryMappedFileName);
		CloseHandle(hMemoryFile);
		return 1;
	}

	LPDWORD dwordBuffer = (LPDWORD)buffer;
	
	if (mutexSynchronization) {
		hMtx = CreateMutex(NULL, true, mutexName);
		if (hMtx == NULL) {
			printf("Failed to create mutex \"%s\"!\n", mutexName);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			return 1;
		}

		*dwordBuffer = 1;
	} else {
		hEventSentPair = CreateEvent(NULL, false, false, "sent-pair");
		if (hEventSentPair == NULL) {
			printf("Failed to open event \"%s\"!\"", event1Name);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			return 1;
		}
		hEventReceivedPair = CreateEvent(NULL, false, true, "received-pair");
		if (hEventReceivedPair == NULL) {
			printf("Failed to open event \"%s\"!\n", event2Name);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			CloseHandle(hEventSentPair);
			return 1;
		}

		*dwordBuffer = 2;
	}

	if (!CreateProcess(program2ExePath, NULL, NULL, NULL, false, 0, NULL, NULL,
			&startupInfo, &processInformation)) {
		printf("Failed to create the second process! target exe = %s\n", program2ExePath);
		CLOSE_HANDLES;
		return 1;
	}

	while (true) {
		std::cout << "----------------------------\n";
		std::cout << "Enter the number of pairs to be sent to the other process:\n";

		std::string line;
		std::getline(std::cin, line);
		trim(line);

		if (line == "stop") {
			break;
		}

		int numberOfPairs = parseStringInt(line);
		if (numberOfPairs == -1 || numberOfPairs > 1000) {
			std::cout << "Invalid number of pairs! It should be less than 1000!\n";
			continue;
		}
		
		for (int tr = 0; tr < numberOfPairs; ++tr) {
			DWORD a = rand();
			DWORD b = a * 2;

			if (!mutexSynchronization) {
				enterCriticalSectionEvents(hEventSentPair, hEventReceivedPair);

				*dwordBuffer = a;
				*(dwordBuffer + 1) = b;
				std::cout << "----Sent: a = " << a << ", b = " << b << '\n';

				exitCriticalSectionEvents(hEventSentPair, hEventReceivedPair);
			} else {

				*dwordBuffer = a;
				*(dwordBuffer + 1) = b;
				std::cout << "----Sent: a = " << a << ", b = " << b << '\n';

				exitCriticalSectionMutex(hMtx);
				enterCriticalSectionMutex(hMtx);
			}
		}

		Sleep(100);
	}

	*dwordBuffer = STOP_FLAG;
	if (mutexSynchronization) {
		enterCriticalSectionMutex(hMtx);
	} else {
		enterCriticalSectionEvents(hEventSentPair, hEventReceivedPair);
	}

	UnmapViewOfFile((LPCVOID*)buffer);
	CLOSE_HANDLES;
	return 0;
}
