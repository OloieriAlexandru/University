#include <windows.h>
#include <iostream>

const char* memoryMappedFileName = "processes_info.info";
const DWORD STOP_FLAG = 10000000;

const char* event1Name = "sent-pair";
const char* event2Name = "received-pair";
const char* mutexName = "mtx";

void enterCriticalSectionEvents(HANDLE hEventSentPair, HANDLE) {
	WaitForSingleObject(hEventSentPair, INFINITE);
	ResetEvent(hEventSentPair);
}

void enterCriticalSectionMutex(HANDLE hMtx) {
	WaitForSingleObject(hMtx, INFINITE);
}

void exitCriticalSectionEvents(HANDLE, HANDLE hEventReceivedPair) {
	SetEvent(hEventReceivedPair);
}

void exitCriticalSectionMutex(HANDLE hMtx) {
	ReleaseMutex(hMtx);
}

int main() {
	HANDLE hEventSentPair = NULL;
	HANDLE hEventReceivedPair = NULL;
	HANDLE hMtx = NULL;

	HANDLE hMemoryFile = OpenFileMapping(
		PAGE_READWRITE,
		true,
		memoryMappedFileName);

	if (hMemoryFile == NULL) {
		printf("Failed to open the \"%s\" memory-mapped file!", memoryMappedFileName);
		return 1;
	}

	auto buffer = (char*)MapViewOfFile(
		hMemoryFile,
		FILE_MAP_READ,
		0, 0, 0);

	if (buffer == NULL) {
		printf("Failed to map view of \"%s\" memory-mapped file!\n", memoryMappedFileName);
		CloseHandle(hMemoryFile);
		return 1;
	}
	LPDWORD dwordBuffer = (LPDWORD)buffer;

	bool mutexSynchronization;

	if (*dwordBuffer == 1) {
		mutexSynchronization = true;
	} else if (*dwordBuffer == 2) {
		mutexSynchronization = false;
	} else {
		printf("Invalid flag DWORD! Expected 1 or 2, found %d\n", *dwordBuffer);
		UnmapViewOfFile((LPCVOID*)buffer);
		CloseHandle(hMemoryFile);
		return 1;
	}

	if (mutexSynchronization) {
		hMtx = OpenMutex(SYNCHRONIZE, false, mutexName);
		if (hMtx == NULL) {
			printf("Failed to open mutex \"%s\"!\n", mutexName);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			return 1;
		}
	} else {
		hEventSentPair = OpenEvent(EVENT_ALL_ACCESS, false, event1Name);
		if (hEventSentPair == NULL) {
			printf("Failed to open event \"%s\"!\"", event1Name);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			return 1;
		}
		hEventReceivedPair = OpenEvent(EVENT_ALL_ACCESS, false, event2Name);
		if (hEventReceivedPair == NULL) {
			printf("Failed to open event \"%s\"!\n", event2Name);
			UnmapViewOfFile((LPCVOID*)buffer);
			CloseHandle(hMemoryFile);
			CloseHandle(hEventSentPair);
			return 1;
		}
	}

	DWORD a;
	DWORD b;

	while (true) {
		if (mutexSynchronization) {
			enterCriticalSectionMutex(hMtx);
		} else {
			enterCriticalSectionEvents(hEventSentPair, hEventReceivedPair);
		}

		a = *dwordBuffer;
		b = *(dwordBuffer + 1);

		if (a == STOP_FLAG) {
			if (mutexSynchronization) {
				exitCriticalSectionMutex(hMtx);
			} else {
				exitCriticalSectionEvents(hEventSentPair, hEventReceivedPair);
			}
			break;
		}

		std::cout << "Received: a = " << a << ", b = " << b << ", " << (a * 2 == b ? "correct" : "incorrect") << '\n';
		
		if (mutexSynchronization) {
			exitCriticalSectionMutex(hMtx);
		} else {
			exitCriticalSectionEvents(hEventSentPair, hEventReceivedPair);
		}
	}

	UnmapViewOfFile((LPCVOID*)buffer);
	CloseHandle(hMemoryFile);
	if (mutexSynchronization) {
		CloseHandle(hMtx);
	} else {
		CloseHandle(hEventSentPair);
		CloseHandle(hEventReceivedPair);
	}
	return 0;
}
