#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>

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

class ProcessInfo {
private:
	DWORD pid;

	DWORD ppid;

	std::string exeName;

public:
	ProcessInfo(DWORD pid, DWORD ppid, const std::string exeName) {
		this->pid = pid;
		this->ppid = ppid;
		this->exeName = exeName;
	}

	DWORD getPid() const {
		return pid;
	}

	DWORD getPPid()  const {
		return ppid;
	}

	const std::string& getExeName() const {
		return exeName;
	}

	friend std::ostream& operator << (std::ostream& out, const ProcessInfo& processInfo) {
		out << processInfo.getPid() << ' ' << processInfo.getPPid() << ' ' << processInfo.getExeName();
		return out;
	}
};

std::vector<unsigned int> deletedProcessesNodes;

std::vector<ProcessInfo> processes;

std::vector<unsigned int> degrees;

std::unordered_map<DWORD, unsigned int> processPidToNodeMap;

std::unordered_map<std::string, std::vector<unsigned int>> processExeToNodeMap;

std::vector<std::vector<int>> graph;

void addProcessInfo(const ProcessInfo& processInfo) {
	if (processPidToNodeMap.count(processInfo.getPid())) {
		return;
	}
	processPidToNodeMap[processInfo.getPid()] = processes.size();
	processExeToNodeMap[processInfo.getExeName()].push_back(processes.size());
	processes.push_back(processInfo);
	degrees.push_back(0u);
	graph.push_back(std::vector<int>());
}

int countNodesFromSubtree(unsigned int node) {
	if (node >= graph.size()) {
		return 0;
	}
	int result = graph[node].size();
	for (auto node : graph[node]) {
		result += countNodesFromSubtree(node);
	}
	return result;
}

void traverseSubtree(unsigned int node, const std::string& tabs) {
	if (node >= graph.size()) {
		return;
	}
	std::cout << tabs << processes[node].getExeName() << ' ' << processes[node].getPid() << '\n';
	for (auto adjacentNode : graph[node]) {
		traverseSubtree(adjacentNode, tabs + std::string("\t"));
	}
}

bool closeProcessSubtree(unsigned int node) {
	if (node >= graph.size()) {
		return true;
	}
	deletedProcessesNodes.push_back(node);
	bool ans = true;
	for (auto adjacentNode : graph[node]) {
		ans = ans && closeProcessSubtree(adjacentNode);
	}
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, processes[node].getPid());
	if (hProcess != NULL) {
		TerminateProcess(hProcess, 9);
		CloseHandle(hProcess);
	}
	else {
		std::cout << "Failed to open the process with pid = " << processes[node].getPid()
			<< ", exe name = " << processes[node].getExeName() << '\n';
		ans = false;
	}
	return ans;
}

void clearProcessInfo(unsigned int nodeIndex) {
	processPidToNodeMap.erase(processes[nodeIndex].getPid());
	processExeToNodeMap.erase(processes[nodeIndex].getExeName());
	for (auto& adjacencyList : graph) {
		int rm = -1;
		for (int i = 0; i < adjacencyList.size(); ++i) {
			if (adjacencyList[i] == nodeIndex) {
				rm = i;
				break;
			}
		}
		if (rm == -1) {
			continue;
		}
		adjacencyList.erase(adjacencyList.begin() + rm);
	}
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

bool enablePrivilege() {
	HANDLE userToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &userToken)) {
		std::cout << "Failed to get the token associated to the current proces, having pid = "
			<< GetCurrentProcess() << '\n';
		return false;
	}
	TOKEN_PRIVILEGES tokenPrivileges;
	LUID luid;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
		std::cout << "Failed to lookup privilege value!\n";
		return false;
	}

	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Luid = luid;
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(
		userToken, false, &tokenPrivileges,
		sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)
		|| GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		std::cout << "Failed to adjust the token privileges!\n";
		return false;
	}

	CloseHandle(userToken);
	return true;
}

int main() {
	std::cout << "Program 2!\n------------------------------\n";

	if (!enablePrivilege()) {
		return 1;
	}

	runUntilRead("written", "Type \"written\" after the execution of the first program has ended!");

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

	char* strBuffer = new char[1 << 10];
	if (!strBuffer) {
		printf("Failed to allocate memory for a temporary string buffer!\n");
		CloseHandle(hMemoryFile);
		return 1;
	}
	auto copyBuffer = strBuffer;

	while (*buffer++) {
		DWORD* dwordPtr = (DWORD*)buffer;
		DWORD pid = *dwordPtr++;
		DWORD ppid = *dwordPtr++;
		DWORD len = *dwordPtr++;
		buffer += 3 * sizeof(DWORD);
		strcpy(strBuffer, buffer);
		buffer += len + 1;

		addProcessInfo(ProcessInfo(pid, ppid, std::string(strBuffer)));
		// std::cout << pid << ' ' << ppid << ' ' << len << ' ' << strBuffer << '\n';
	}
	UnmapViewOfFile((LPCVOID*)copyBuffer);

	for (auto process : processes) {
		unsigned int x = processPidToNodeMap[process.getPPid()];
		unsigned int y = processPidToNodeMap[process.getPid()];
		if (x == y || !x || !y) {
			continue;
		}
		graph[x].push_back(y);
		++degrees[y];
	}
	std::cout << "----------------------------\n";
	unsigned int tr = 1;
	for (unsigned int i = 0; i < graph.size(); ++i) {
		if (degrees[i]) {
			continue;
		}
		std::cout << "[Arbore nr. " << tr++ << "]\n";
		traverseSubtree(i, "");
	}

	std::vector<std::string> options = {
		"----------------------------",
		"Option 1 - {{process-name}}",
		"Option 2 - {{process-pid}}" };

	while (true) {
		for (auto option : options) {
			std::cout << option << '\n';
		}

		std::string line;
		std::getline(std::cin, line);
		trim(line);

		if (line.find("Option 1 - ") == 0) {
			std::string processName = line.substr(11);
			if (processExeToNodeMap.count(processName)) {
				unsigned int number = 1;
				for (auto nodeIndex : processExeToNodeMap[processName]) {
					std::cout << number++ << ". " << processName << ", pid = " << processes[nodeIndex].getPid()
						<< ", descendants count = " << countNodesFromSubtree(nodeIndex) << '\n';
				}
			}
			else {
				std::cout << "Invalid process name!\n";
			}
		}
		else if (line.find("Option 2 - ") == 0) {
			std::string pidStr = line.substr(11);
			int parsedPidStr = parseStringInt(pidStr);
			if (parsedPidStr == -1 || !processPidToNodeMap.count(parsedPidStr)) {
				std::cout << "Invalid process pid!\n";
			}
			else {
				deletedProcessesNodes.clear();
				if (closeProcessSubtree(processPidToNodeMap[parsedPidStr])) {
					std::cout << "All the processes from the subtree of the process with pid = " << parsedPidStr << " were closed!\n";
				}
				for (auto deletedProcessNode : deletedProcessesNodes) {
					clearProcessInfo(deletedProcessNode);
				}
			}
		}
		else {
			std::cout << "Invalid option!\n";
		}
	}

	CloseHandle(hMemoryFile);
	delete[] strBuffer;
	return 0;
}
