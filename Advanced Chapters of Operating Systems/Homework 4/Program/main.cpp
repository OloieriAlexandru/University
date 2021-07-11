/*
Documentatie: 
http://students.info.uaic.ro/~george.popoiu/laborator5.pdf
https://docs.microsoft.com/en-us/windows/win32/api/wininet/nf-wininet-ftpputfilea
https://docs.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathstrippatha
*/

/*
Fisier:
4
127.0.0.1
alexandru
parola123
PUT C:\windows\system32\calc.exe
RUN calc.exe
PUT C:\windows\system32\cmd.exe
RUN cmd.exe
*/

#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <shlwapi.h>

#pragma comment(lib, "Wininet")
#pragma comment(lib, "Shlwapi.lib")

#define BUFFER_SIZE 256

// const char* httpServerFileUrl = "http://165.227.141.114:8889/info.txt";
const char* httpServerFileUrl = "https://raw.githubusercontent.com/OloieriAlexandru/CSSO-H4/master/info.txt";

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

HINTERNET openInternet() {
	HINTERNET hInternet;
	if ((hInternet = InternetOpen("Alexandru Oloieri", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL)) == NULL) {
		printf("Function Call \"InternetOpen\" failed!\n");
		exit(1);
	}
	return hInternet;
}

std::string httpGetFile(const char* url) {
	if (url == nullptr) {
		printf("Invalid arguments provided for \"httpGetFile\" function!\n");
		exit(1);
	}

	HINTERNET hInternet = openInternet();

	HINTERNET hUrl;
	if ((hUrl = InternetOpenUrl(hInternet, url, NULL, 0, 0, 0)) == NULL) {
		printf("Function Call \"InternetOpenUrl\" failed!\n");
		InternetCloseHandle(hInternet);
		exit(1);
	}

	std::string httpBody;
	char buffer[BUFFER_SIZE];
	DWORD numberOfBytesRead;

	while (true) {
		if (InternetReadFile(hUrl, buffer, BUFFER_SIZE-1, &numberOfBytesRead)) {
			if (numberOfBytesRead == 0) {
				break;
			}
			buffer[numberOfBytesRead] = '\0';
			httpBody += std::string(buffer);
		} else {
			printf("Function Call \"InternetReadFile\" failed!\n");
			exit(1);
		}
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	return httpBody;
}

std::vector<std::string> splitByDelimiter(const std::string str, char delimiter) {
	std::vector<std::string> strs;

	int idx = 0;
	std::string buf;
	while (idx < str.size()) {
		if (str[idx] == delimiter) {
			if (buf.size()) {
				strs.push_back(buf);
			}
			buf.clear();
		} else {
			buf.push_back(str[idx]);
		}
		++idx;
	}

	return strs;
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

void executeFtpCommands(const std::vector<std::string>& commands, const std::string& ftpAddress, 
	const std::string& ftpUsername, const std::string& ftpPassword) {
	HINTERNET hInternet = openInternet();

	HINTERNET hFtp;
	if ((hFtp = InternetConnect(hInternet, ftpAddress.c_str(), INTERNET_DEFAULT_FTP_PORT, ftpUsername.c_str(),
								ftpPassword.c_str(), INTERNET_SERVICE_FTP, NULL, NULL)) == NULL) {
		InternetCloseHandle(hInternet);
		printf("Function Call \"InternetConnect\" failed!\n");
		std::cout << "Cause: " << GetLastError() << '\n';
		exit(1);
	}

	int commandNumber = 0;
	for (auto& command : commands) {
		++commandNumber;
		std::string commandType;
		std::string commandArg;

		int idx = 0;
		for (idx = 0; idx < command.size(); ++idx) {
			if (command[idx] == ' ') {
				break;
			}
			commandType.push_back(command[idx]);
		}
		++idx;
		for (; idx < command.size(); ++idx) {
			commandArg.push_back(command[idx]);
		}

		trim(commandArg);

		if (!(commandType == "PUT" || commandType == "RUN")) {
			printf("Invalid %d th command: invalid command type!\n", commandNumber);
			continue;
		}
		if (commandArg.size() == 0 || commandArg.size() > MAX_PATH) {
			printf("Invalid %d th command: invalid command argument!\n", commandNumber);
			continue;
		}

		if (commandType == "PUT") {
			char fileName[MAX_PATH + 1];
			strcpy(fileName, commandArg.c_str());
			PathStripPath(fileName);

			if (!FtpPutFile(hFtp, commandArg.c_str(), fileName, FTP_TRANSFER_TYPE_BINARY, NULL)) {
				InternetCloseHandle(hFtp);
				InternetCloseHandle(hInternet);

				printf("Failed to send \"%s\" file to ftp server!\n", commandArg.c_str());
				std::cout << "Cause: " << GetLastError() << '\n';
				exit(1);
			}

			printf("\"%s\" file successfully sent!\n", commandArg.c_str());
		} else {
			std::string localFilePath = std::string("C:\\Users\\olo\\Desktop\\Filezilla_downloads\\") + commandArg;

			if (!FtpGetFile(hFtp, commandArg.c_str(), localFilePath.c_str(), false, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY, NULL)) {
				InternetCloseHandle(hFtp);
				InternetCloseHandle(hInternet);

				printf("Failed to download \"%s\" file from the ftp server!\n", commandArg.c_str());
				std::cout << "Cause: " << GetLastError() << '\n';
				exit(1);
			}

			printf("\"%s\" file successfully downloaded!\n", localFilePath.c_str());

			PROCESS_INFORMATION processInformation;
			STARTUPINFO startupInfo;
			memset(&startupInfo, 0, sizeof(startupInfo));
			startupInfo.cb = sizeof(startupInfo);

			if (!CreateProcess(localFilePath.c_str() , NULL, NULL, NULL, false,
				CREATE_NEW_CONSOLE, // flag ce indica deschiderea unei console noi cand se ruleaza cmd.exe
				NULL, NULL,
				&startupInfo, &processInformation)) {
				InternetCloseHandle(hFtp);
				InternetCloseHandle(hInternet);

				printf("Failed to create a process! Target exe = %s\n", localFilePath.c_str());
				exit(1);
			}

			CloseHandle(processInformation.hProcess);
			CloseHandle(processInformation.hThread);

			printf("\"%s\" file successfully started!\n", localFilePath.c_str());
		}
	}

	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
}

int main() {
	const int infoLinesCount = 4;

	std::vector<std::string> fileLines = splitByDelimiter(httpGetFile(httpServerFileUrl), '\n');
	
	// Minimum: 0 commands, ftp address, username, password
	if (fileLines.size() < infoLinesCount) {
		printf("Invalid file read from the http server! It should have at least %d lines!\n", infoLinesCount);
		exit(1);
	}

	int numberOfCommands = parseStringInt(fileLines[0]);
	if (numberOfCommands == -1) {
		printf("Invalid file read from the http server! Expecting an integer on the first line!\n");
		exit(1);
	}

	std::string ftpAddress = fileLines[1];
	std::string ftpUsername = fileLines[2];
	std::string ftpPassword = fileLines[3];

	std::reverse(fileLines.begin(), fileLines.end());
	for (int i = 0; i < infoLinesCount; ++i) {
		fileLines.pop_back();
	}
	std::reverse(fileLines.begin(), fileLines.end());

	if (fileLines.size() != numberOfCommands) {
		printf("Invalid file read from the http server! The integer on the first line doesn't match the number of commands in the file!\n");
		exit(1);
	}

	executeFtpCommands(fileLines, ftpAddress, ftpUsername, ftpPassword);
	return 0;
}
