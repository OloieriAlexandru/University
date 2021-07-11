#include <iostream>
#include <windows.h>
#include <Tlhelp32.h>
#include <cstdio>
#include <string>

// Documentatie:
// https://profs.info.uaic.ro/~vidrascu/CSSO/resurse/WinAPI/laborator3.pdf
// https://www.tek-tips.com/viewthread.cfm?qid=715415

const char* memoryMappedFileName = "processes_info.info";

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

void runUntilRead(std::string expectedString, std::string message) {
	while (true) {
		std::cout << message << '\n';
		std::string line;
		std::getline(std::cin, line);
		trim(line);
		if (line == expectedString) {
			break;
		}
	}
}

int main() {
	std::cout << "Program 1!\n------------------------------\n";
	std::cout << "Starting to get the details of the processes and to write them to a memory-mapped file!\n";

	HANDLE hProcess;
	PROCESSENTRY32 pe32;

	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcess == INVALID_HANDLE_VALUE) {
		printf("CreateToolhelp32Snapshot failed.err = %d \n", GetLastError());
		return 1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcess, &pe32)) {
		printf("Process32First failed. err = %d\n", GetLastError());
		CloseHandle(hProcess);
		return 1;
	}

	HANDLE hMemoryFile = CreateFileMapping(
		NULL,
		NULL,
		PAGE_READWRITE,
		0,
		1 << 16,
		memoryMappedFileName);

	if (hMemoryFile == NULL) {
		printf("Failed to create the \"%s\" memory-mapped file!\n", memoryMappedFileName);
		CloseHandle(hProcess);
		return 1;
	}

	auto buffer = (char*)MapViewOfFile(
		hMemoryFile,
		FILE_MAP_WRITE,
		0, 0, 0);
	auto copyBuffer = buffer;

	if (buffer == NULL) {
		printf("Failed to map view of \"%s\" memory-mapped file!\n", memoryMappedFileName);
		CloseHandle(hProcess);
		CloseHandle(hMemoryFile);
		return 1;
	}

	int index = 0;
	do {
		DWORD pid = pe32.th32ProcessID;
		DWORD ppid = pe32.th32ParentProcessID;
		auto exe = pe32.szExeFile;

		*buffer++ = '\1';
		DWORD* ptr = (DWORD*)buffer;
		DWORD len = strlen(exe);

		*ptr++ = pid;
		*ptr++ = ppid;
		*ptr++ = len;
		CHAR* ptrStr = (CHAR*)ptr;
		strcpy(ptrStr, exe);
		buffer = (char*)ptrStr + (len + 1);

		std::cout << ++index << ". " << pid << ' ' << ppid << ' ' << exe << '\n';
	} while (Process32Next(hProcess, &pe32));
	*buffer = '\0';

	std::cout << "Finished writing the information about the processes to the memory-mapped file!\n"
		"------------------------------\n";
	runUntilRead("read", "Type \"read\" after the second program has read the processes info from the memory-mapped file!");

	UnmapViewOfFile((LPCVOID*)copyBuffer);
	CloseHandle(hProcess);
	CloseHandle(hMemoryFile);
	return 0;
}
