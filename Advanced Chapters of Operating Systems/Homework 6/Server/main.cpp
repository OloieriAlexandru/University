#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <wininet.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wininet")

/*
Documentation:
https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recvfrom
https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeya

https://stackoverflow.com/questions/3828835/how-can-we-check-if-a-file-exists-or-not-using-win32-program
*/

const unsigned short PORT = 4444;

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

SOCKET getServerSocket() {
	int iResult;
	SOCKET sd;
	struct sockaddr_in RecvAddr;

	//-----------------------------------------------
	// Create a receiver socket to receive datagrams
	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sd == INVALID_SOCKET) {
		printf("socket failed with error %d\n", WSAGetLastError());
		exit(1);
	}

	//-----------------------------------------------
	// Bind the socket to any address and the specified port.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(PORT);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iResult = bind(sd, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult != 0) {
		printf("bind failed with error %d\n", WSAGetLastError());
		exit(1);
	}

	return sd;
}

void close(SOCKET sd) {
	int iResult;

	//-----------------------------------------------
	// Close the socket when finished receiving datagrams
	wprintf(L"Finished receiving. Closing socket.\n");
	iResult = closesocket(sd);
	if (iResult == SOCKET_ERROR) {
		printf("closesocket failed with error %d\n", WSAGetLastError());
		exit(1);
	}

	//-----------------------------------------------
	// Clean up and exit.
	WSACleanup();
}

std::string requestResponse;

void tokenize(char buffer[], int len, std::vector<std::string>& tokens) {
	std::string token;
	bool doubleQuote = false;

	tokens.clear();
	for (int i = 0; i < len; ++i) {
		if (buffer[i] == ' ') {
			if (!doubleQuote) {
				if (token.size() > 0) {
					tokens.push_back(token);
					token.clear();
				}
			} else {
				token.push_back(buffer[i]);
			}
		} else if (buffer[i] == '\"') {
			if (doubleQuote) {
				tokens.push_back(token);
				token.clear();
			}
			doubleQuote = !doubleQuote;
		} else {
			token.push_back(buffer[i]);
		}
	}
	if (token.size() > 0) {
		tokens.push_back(token);
	}
}

// ---------------------- Commands Specific Code Start

bool fileExists(const char* filePath) {
	DWORD dwAttrib = GetFileAttributes(filePath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool directoryExists(const char* dirPath) {
	DWORD dwAttrib = GetFileAttributes(dirPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

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

HANDLE openFileAppendMode(const char* fileName) {
	return CreateFile(
		fileName,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		INVALID_HANDLE_VALUE);
}

HANDLE openFileRead(const char* fileName) {
	return CreateFile(
		fileName,
		GENERIC_READ,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		INVALID_HANDLE_VALUE);
}

const HKEY registryHKey = HKEY_LOCAL_MACHINE;

bool registryKeyExists(const char* regKey) {
	HKEY key;
	int err = RegOpenKeyEx(registryHKey, regKey, 0, KEY_READ, &key);
	if (err == ERROR_SUCCESS) {
		RegCloseKey(key);
		return true;
	}
	return false;
}

bool createRegValue(HKEY regKey, const char* regValueName, const std::string& regValueValue) {
	if (RegSetValueEx(
		regKey,
		regValueName,
		0,
		REG_SZ,
		(const BYTE*)regValueValue.c_str(),
		regValueValue.size() + 1)) {
		requestResponse += std::string("Failed to create registry value: ") + std::string(regValueName);
		return false;
	}
	return true;
}

bool deleteRegValue(HKEY regKey, const char* regValueName) {
	bool res = (RegDeleteValue(regKey, regValueName) == ERROR_SUCCESS);
	if (!res) {
		requestResponse += "Failed to delete registry value:" + std::string(regValueName);
	}
	return res;
}

bool openRegistryKey(HKEY registryHKey, const char* regKeyName, HKEY& rKey) {
	if (RegOpenKey(registryHKey, regKeyName, &rKey) != ERROR_SUCCESS) {
		requestResponse += "Failed to open registry key: " + std::string(regKeyName);
		return false;
	}
	return true;
}

bool startProcess(const char* exePath) {
	PROCESS_INFORMATION processInformation;
	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	if (!CreateProcess(exePath, NULL, NULL, NULL, false,
		CREATE_NEW_CONSOLE,
		NULL, NULL,
		&startupInfo, &processInformation)) {
		requestResponse += "Failed to create a process! Target exe = %s" + std::string(exePath);
		return false;
	}

	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);
	return true;
}

bool openInternet(HINTERNET& hInternet) {
	if ((hInternet = InternetOpen("Alexandru Oloieri", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL)) == NULL) {
		requestResponse += "Function Call \"InternetOpen\" failed!";
		return false;
	}
	return true;
}

const int BUFFER_SIZE = 1024;

bool httpGetFile(const char* url, std::string& httpBody) {
	HINTERNET hInternet;
	if (!openInternet(hInternet)) {
		return false;
	}

	HINTERNET hUrl;
	if ((hUrl = InternetOpenUrl(hInternet, url, NULL, 0, 0, 0)) == NULL) {
		requestResponse += "Function Call \"InternetOpenUrl\" failed!";
		InternetCloseHandle(hInternet);
		return false;
	}

	char buffer[BUFFER_SIZE];
	DWORD numberOfBytesRead;

	while (true) {
		if (InternetReadFile(hUrl, buffer, BUFFER_SIZE - 1, &numberOfBytesRead)) {
			if (numberOfBytesRead == 0) {
				break;
			}
			buffer[numberOfBytesRead] = '\0';
			httpBody += std::string(buffer);
		}
		else {
			requestResponse += "Function Call \"InternetReadFile\" failed!";
			return false;
		}
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	return true;
}

void recursiveListDirectory(const char* dirPath, const std::string& prefix) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string searchPath = std::string(dirPath) + std::string("\\*");
	hFind = FindFirstFile(searchPath.c_str(), &ffd);

	do {
		std::string strFileName = std::string(ffd.cFileName);
		if (strFileName == "." || strFileName == "..") {
			continue;
		}
		std::string fullPath = std::string(dirPath) + "\\" + strFileName;
		std::string currentStr = prefix + fullPath + std::string("\n");
		requestResponse += currentStr;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			recursiveListDirectory(fullPath.c_str(), prefix + std::string("\t"));
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

const char* localDirPath = "D:\\FII\\CSSO6Dir\\";

// ---------------------- Commands Specific Code End

void invalidRequest() {
	requestResponse += "Invalid command!";
}

void handleReadFile(const std::vector<std::string>& tokens) {
	std::string fileName = std::string(localDirPath) + tokens[1];
	if (!fileExists(fileName.c_str())) {
		requestResponse += "The file doesn't exist!";
		return;
	}
	HANDLE fHandle = openFileRead(fileName.c_str());
	if (fHandle == INVALID_HANDLE_VALUE) {
		requestResponse += "There was an error when opening the file!";
		return;
	}

	char buffer[BUFFER_SIZE + 1];
	DWORD read;
	while (ReadFile(fHandle, buffer, BUFFER_SIZE, &read, NULL)) {
		if (read == 0) {
			break;
		}
		buffer[read] = '\0';
		requestResponse += std::string(buffer);
	}

	CloseHandle(fHandle);
}

void handleCreateFile(const std::vector<std::string>& tokens) {
	std::string fileName = std::string(localDirPath) + tokens[1];
	if (fileExists(fileName.c_str())) {
		requestResponse += "The file already exists!";
		return;
	}
	HANDLE fHandle = createFile(fileName.c_str());
	if (fHandle == INVALID_HANDLE_VALUE) {
		requestResponse += "There was an error when creating the file!";
		return;
	}
	CloseHandle(fHandle);
	requestResponse += "File created successfully!";
}

void handleAppendToFile(const std::vector<std::string>& tokens) {
	if (tokens.size() != 3) {
		invalidRequest();
		return;
	}
	std::string fileName = std::string(localDirPath) + tokens[1];
	if (!fileExists(fileName.c_str())) {
		requestResponse += "The file doesn't exist!";
		return;
	}
	HANDLE fHandle = openFileAppendMode(fileName.c_str());
	if (fHandle == INVALID_HANDLE_VALUE) {
		requestResponse += "There was an error when opening the file!";
		return;
	}
	DWORD bytesWritten = 0;
	if (!WriteFile(fHandle, tokens[2].c_str(), tokens[2].size(), &bytesWritten, NULL)) {
		requestResponse += "There was an error when writing the string to the file!";
		return;
	}
	CloseHandle(fHandle);
	requestResponse += "String successfully appended to file!";
}

void handleDeleteFile(const std::vector<std::string>& tokens) {
	std::string fileName = std::string(localDirPath) + tokens[1];
	if (!fileExists(fileName.c_str())) {
		requestResponse += "Invalid file name!";
		return;
	}
	if (!DeleteFile(fileName.c_str())) {
		requestResponse += "There was an error when deleting the file!";
		return;
	}
	requestResponse += "The file was deleted successfully!";
}

void handleCreateReg(const std::vector<std::string>& tokens) {
	if (registryKeyExists(tokens[1].c_str())) {
		requestResponse += "Registry key already exists!";
		return;
	}

	HKEY hKey;
	DWORD disposition;
	DWORD errorCode;

	printf("%s\n", tokens[1].c_str());
	if ((errorCode = RegCreateKeyEx(
		registryHKey,
		tokens[1].c_str(),
		0, NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hKey,
		&disposition)) != ERROR_SUCCESS) {
		requestResponse += "Error when calling \"RegCreateKeyEx()\"!";
		return;
	}
	RegCloseKey(hKey);
	requestResponse += "Registry key created successfully!";
}

void handleDeleteReg(const std::vector<std::string>& tokens) {
	if (!registryKeyExists(tokens[1].c_str())) {
		requestResponse += "Registry key doesn't exist!";
		return;
	}
	if (ERROR_SUCCESS != RegDeleteKey(registryHKey, tokens[1].c_str())) {
		requestResponse += "Failed to delete the registry key!";
		return;
	}
	requestResponse += "Registry key deleted successfully!";
}

void handleCreateRegValue(const std::vector<std::string>& tokens) {
	if (tokens.size() < 3) {
		invalidRequest();
		return;
	}
	if (!registryKeyExists(tokens[1].c_str())) {
		requestResponse += "Registry key doesn't exist!";
		return;
	}
	HKEY rKey;
	if (!openRegistryKey(registryHKey, tokens[1].c_str(), rKey)) {
		return;
	}
	std::string regValueValue = "";
	if (tokens.size() >= 4) {
		regValueValue = std::string(tokens[3]);
	}
	if (!createRegValue(rKey, tokens[2].c_str(), regValueValue)) {
		return;
	}
	requestResponse += "Registry value created successfully!";
}

void handleDeleteRegValue(const std::vector<std::string>& tokens) {
	if (tokens.size() < 3) {
		invalidRequest();
		return;
	}
	if (!registryKeyExists(tokens[1].c_str())) {
		requestResponse += "Registry key doesn't exist!";
		return;
	}
	HKEY rKey;
	if (!openRegistryKey(registryHKey, tokens[1].c_str(), rKey)) {
		return;
	}
	if (!deleteRegValue(rKey, tokens[2].c_str())) {
		return;
	}
	requestResponse += "Registry value deleted successfully!";
}

void handleRun(const std::vector<std::string>& tokens) {
	std::string path;
	if (!fileExists(tokens[1].c_str())) {
		std::string newTryPath = std::string(localDirPath) + tokens[1];
		if (!fileExists(newTryPath.c_str())) {
			requestResponse += "Invalid executable path!";
			return;
		}
		path = newTryPath;
	} else {
		path = tokens[1];
	}
	if (!startProcess(path.c_str())) {
		return;
	}
	requestResponse += "Process started successfully!";
}

void handleDownload(const std::vector<std::string>& tokens) {
	std::string response;
	if (!httpGetFile(tokens[1].c_str(), response)) {
		return;
	}
	requestResponse += response;
}

void handleList(const std::vector<std::string>& tokens) {
	if (!directoryExists(tokens[1].c_str())) {
		requestResponse += "The specified path is not a directory!";
		return;
	}
	recursiveListDirectory(tokens[1].c_str(), std::string(""));
	if (requestResponse.size()) {
		requestResponse.pop_back();
	}
}

const std::vector<std::string> commands = {
	"readfile",
	"createfile",
	"appendtofile",
	"deletefile",
	"createreg",
	"deletereg",
	"createregvalue",
	"deleteregvalue",
	"run",
	"download",
	"list",
	"listcmd"
};

void handleListCmd(const std::vector<std::string>& tokens) {
	for (int i = 0; i < commands.size(); ++i) {
		requestResponse += commands[i];
		if (i + 1 < commands.size()) {
			requestResponse.push_back('\n');
		}
	}
}

const int HANDLERS_COUNT = 12;
void (*handleCommand[HANDLERS_COUNT])(const std::vector<std::string>&) = {
	handleReadFile,
	handleCreateFile,
	handleAppendToFile,
	handleDeleteFile,
	handleCreateReg,
	handleDeleteReg,
	handleCreateRegValue,
	handleDeleteRegValue,
	handleRun,
	handleDownload,
	handleList,
	handleListCmd
};

void strToLowercase(std::string& str) {
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] -= 'A';
			str[i] += 'a';
		}
	}
}

int getCommandIndex(std::vector<std::string>& tokens) {
	if (HANDLERS_COUNT != commands.size()) {
		printf("INTERNAL ERROR! The number of handlers pointers doesn't match the number of commands!\n");
		exit(2);
	}
	if (tokens.size() < 1) {
		return -1;
	}
	strToLowercase(tokens[0]);
	for (int i = 0; i < commands.size(); ++i) {
		if (commands[i] == tokens[0]) {
			return i;
		}
	}
	printf("%s\n", tokens[0].c_str());
	return -1;
}

const int BUFFER_SIZE_REQ = 2048;

struct RequestParams {
	struct sockaddr_in SenderAddr;
	int SenderAddrSize;
	int bufferLen;
	int sd;
	char buffer[BUFFER_SIZE_REQ + 1];
};

/*
Commands:

readfile fileName
createfile fileName 
appendToFile fileName contentToAppend
deleteFile fileName
createReg regName
deleteReg regName
createRegValue regName regValueName regValueContent
deleteRegValue regName regValueName
run processName
download url
list dirPath
listcmd
*/
DWORD WINAPI handleRequest(LPVOID param) {
	RequestParams* rqParams = (RequestParams*)param;

	std::vector<std::string> tokens;
	tokenize(rqParams->buffer, rqParams->bufferLen, tokens);

	requestResponse.clear();
	int commandIndex = getCommandIndex(tokens);
	if (commandIndex == -1 
		|| !(0 <= commandIndex && commandIndex < HANDLERS_COUNT)) {
		invalidRequest();
	} else if (commands.back() != tokens[0] && tokens.size() < 2) {
		invalidRequest();
	} else {
		handleCommand[commandIndex](tokens);
	}

	int iResult = sendto(rqParams->sd, requestResponse.c_str(), requestResponse.size(),
		0, (SOCKADDR*)&rqParams->SenderAddr, rqParams->SenderAddrSize);
	if (iResult == SOCKET_ERROR) {
		printf("Failed to send back the response");
	}

	delete rqParams;
	return 0;
}

int main() {
	char buffer[BUFFER_SIZE_REQ + 1];

	init();
	SOCKET sd = getServerSocket();

	printf("-------------------------\n");
	printf("Server listening on port: %d\n", PORT);
	bool running = true;
	while (running) {
		//-----------------------------------------------
		// Call the recvfrom function to receive datagrams
		// on the bound socket.
		struct sockaddr_in SenderAddr;
		int SenderAddrSize = sizeof(SenderAddr);

		memset(buffer, 0, sizeof(buffer));
		int iResult = recvfrom(sd,
			buffer, BUFFER_SIZE, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		printf("------------------\n");
		if (iResult == SOCKET_ERROR) {
			printf("recvfrom failed with error %d\n", WSAGetLastError());
			continue;
		}
		buffer[iResult] = '\0';
		printf("Received datagram!\n");
		printf("Content: \"%s\"\n", buffer);

		RequestParams* params = new RequestParams;
		memcpy(params->buffer, buffer, BUFFER_SIZE + 1);
		params->bufferLen = iResult;
		params->SenderAddr = SenderAddr;
		params->SenderAddrSize = SenderAddrSize;
		params->sd = sd;

		HANDLE hThread = CreateThread(
			NULL,
			0,
			&handleRequest,
			(LPVOID)params,
			0,
			NULL);
		if (hThread == NULL) {
			printf("Failed to create a thread for handling the request!\n");
		} else {
			CloseHandle(hThread);
		}
	}

	close(sd);
	return 0;
}
