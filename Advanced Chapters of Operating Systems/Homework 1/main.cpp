#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

// MSDN Documentation I used:
// https://docs.microsoft.com/en-us/windows/win32/sysinfo/enumerating-registry-subkeys

const HKEY registryHKey = HKEY_LOCAL_MACHINE;

const char* startKey = "tema1";
const char* registrySubPath = "SOFTWARE\\CSSO";
const char* targetPath = "D:\\FII";

void quit(unsigned exitCode, const std::string message = "") {
	if (message != "") {
		std::cout << message << '\n';
	}
	exit(exitCode);
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

bool createDirectory(const std::string& directoryPath) {
    auto retCode = CreateDirectory( directoryPath.c_str(), NULL );
    if (retCode != ERROR_PATH_NOT_FOUND) {
        return true;
    }
    return false;
}

void recursiveCreate(HKEY currentKey, std::string registerPath, std::string dirPath, std::string depthStr) {
    DWORD cSubKeys = 0;         // number of subkeys 
    DWORD cbMaxSubKey = 0;      // longest subkey size 
    DWORD cValues = 0;          // number of values for key 
    DWORD cchMaxValue = 0;      // longest value name 

    if (!createDirectory(dirPath)) {
        std::cout << "Error! The directory " << dirPath << " could not be created!\n";
        return;
    }

    DWORD retCode = RegQueryInfoKey(
        currentKey,     // key handle 
        NULL,           // buffer for class name 
        NULL,           // size of class string 
        NULL,           // reserved 
        &cSubKeys,      // number of subkeys 
        &cbMaxSubKey,   // longest subkey size 
        NULL,           // longest class string 
        &cValues,       // number of values for this key 
        &cchMaxValue,   // longest value name 
        NULL,           // longest value data 
        NULL,           // security descriptor 
        NULL );         // last write time 

    if (retCode != ERROR_SUCCESS) {
        std::cout << depthStr << ' ';
        std::cout << "Failed to get information about the register with path: " << registerPath << '\n';
        return;
    }

    ++cbMaxSubKey;
    ++cchMaxValue;
    TCHAR* achKey = new TCHAR[cbMaxSubKey]; // buffer for the longest subkey
    TCHAR* achValue = new TCHAR[cchMaxValue]; // buffer for the longest value name
    if (achKey == nullptr || achValue == nullptr) {
        if (achKey != nullptr) {
            delete[] achKey;
        }
        std::cout << depthStr << ' ';
        std::cout << "Failed to allocate memory for the longest subkey and longest value name buffers!\n";
        return;
    }
    DWORD bufferSize = 0;
    DWORD i;

    if (cSubKeys) {
        for (i = 0; i < cSubKeys; i++) {
            bufferSize = cbMaxSubKey + 1;
            retCode = RegEnumKeyEx(currentKey, i,
                achKey,
                &bufferSize,
                NULL, NULL, NULL, NULL);

            if (retCode == ERROR_SUCCESS) {
                HKEY recKey;
                std::string newRegistrySubPath = registerPath + std::string("\\") + std::string(achKey);
                retCode = RegOpenKeyEx(
                    registryHKey,
                    newRegistrySubPath.c_str(),
                    REG_OPTION_NON_VOLATILE,
                    KEY_READ,
                    &recKey );

                if (retCode == ERROR_SUCCESS) {
                    std::cout << depthStr << ' ';
                    std::cout << "Found registry key: " << newRegistrySubPath << ", entering recursive call!\n";
                    recursiveCreate(recKey, newRegistrySubPath,
                        dirPath + std::string("\\") + std::string(achKey), depthStr + std::string("-"));

                    RegCloseKey(recKey);
                } else {
                    std::cout << depthStr << ' ';
                    std::cout << "Failed to open the registry key with path: " << newRegistrySubPath << '\n';
                }
            } else {
                std::cout << depthStr << ' ';
                std::cout << "Failed to get information about the " << (i + 1)
                    << "th key from the register with path: " << registerPath << '\n';
            }
        }
    }

    if (cValues) {
        for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) {
            bufferSize = cchMaxValue;
            achValue[0] = '\0';
            retCode = RegEnumValue( currentKey, i, achValue,
                &bufferSize,
                NULL, NULL, NULL, NULL );

            if ( retCode == ERROR_SUCCESS ) {
                std::string filePath = dirPath + std::string("\\") + std::string(achValue);

                std::cout << depthStr << ' ';
                std::cout << "Found registry value: " << filePath << ", creating file!\n";

                CloseHandle(createFile(filePath.c_str()));
            } else {
                std::cout << depthStr << ' ';
                std::cout << "Failed to get information about the " << (i + 1)
                    << "th value from the register with path: " << registerPath << '\n';
            }
        }
    }

    if (achKey) {
        delete[] achKey;
    }
    if (achValue) {
        delete[] achValue;
    }
}

int main() {
	DWORD errorCode;
	HKEY key;

    std::string startRegistrySubPath = registrySubPath + std::string("\\") + std::string(startKey);
    std::string startTargetPath = targetPath + std::string("\\") + std::string(startKey);

	if ( ERROR_SUCCESS != ( errorCode = RegOpenKeyEx(
		registryHKey,
        startRegistrySubPath.c_str(),
		REG_OPTION_NON_VOLATILE,
		KEY_READ,
		&key
		) ) ) {
		quit(1, std::string("Failed to open the initial registry key: ") + startRegistrySubPath);
	}

	recursiveCreate(key, startRegistrySubPath, startTargetPath, std::string("-"));

	RegCloseKey(key);
	return 0;
}
