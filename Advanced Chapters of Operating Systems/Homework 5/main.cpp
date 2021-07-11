#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <utility>
#include <vector>
#include <string>
#include <cstdlib>

#include <windows.h>
#include <accctrl.h>
#include <aclapi.h>

/*
Documentation:

https://stackoverflow.com/questions/44199379/reading-registry-in-both-64-and-32-bit-windows
https://stackoverflow.com/questions/29115493/how-to-use-regcreatekeyex
https://www.installsetupconfig.com/win32programming/accesscontrollistaclexample6_2.html
*/

const HKEY	registryHKey = HKEY_LOCAL_MACHINE;
auto*		registrySubPath = "SOFTWARE\\CSSO\\tema5";

const WELL_KNOWN_SID_TYPE ownerSidType = WinCreatorOwnerSid;
const WELL_KNOWN_SID_TYPE allUsersSidType = WinWorldSid;

std::vector<std::pair<std::string, std::string> > regValuesToCreate = {
	{ "key1", "str1" },
	{ "key2", "str2" }
};

void createRegValue(HKEY regKey, const char* regValueName, const std::string& regValueValue) {
	if (RegSetValueEx(
		regKey,
		regValueName,
		0,
		REG_SZ,
		(const BYTE*)regValueValue.c_str(),
		regValueValue.size() + 1
	)) {
		printf("Failed to create registry value: %s\\%s\n", regValueName, regValueValue.c_str());
		return;
	}
}

void initSid(WELL_KNOWN_SID_TYPE type, PSID& pSid) {
	DWORD sz = SECURITY_MAX_SID_SIZE;
	if (!CreateWellKnownSid(
		type,
		NULL,
		pSid,
		&sz
	)) {
		printf("Error when calling \"CreateWellKnownSid()\"!\n");
		printf("Error: %d\n", GetLastError());
		exit(1);
	}
}

int main() {
	HKEY				hKey;
	DWORD				disposition;
	SECURITY_ATTRIBUTES	securityAttributes;

	memset(&securityAttributes, 0, sizeof(SECURITY_ATTRIBUTES));
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = false;
	securityAttributes.lpSecurityDescriptor = new BYTE[SECURITY_DESCRIPTOR_MIN_LENGTH];

	if (!InitializeSecurityDescriptor(securityAttributes.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)) {
		printf("Error when calling \"InitializeSecurityDescriptor()\"!\n");
		return 1;
	}

	PSID ownerSid = new BYTE[SECURITY_MAX_SID_SIZE];
	PSID usersSid = new BYTE[SECURITY_MAX_SID_SIZE];

	if (ownerSid == nullptr || usersSid == nullptr) {
		printf("Failed to allocate memory for owner and users SIDs!\n");
		return 1;
	}

	initSid(ownerSidType, ownerSid);
	initSid(allUsersSidType, usersSid);

	PSID			oloSid = new BYTE[SECURITY_MAX_SID_SIZE];
	DWORD			sz = SECURITY_MAX_SID_SIZE;
	DWORD			domainSize = 1024;
	LPTSTR			domain = new CHAR[1024];
	SID_NAME_USE	use;

	CHAR	username[256];
	DWORD	usernameLen = 256;

	if (!GetUserName(username, &usernameLen)) {
		printf("Error when calling \"\"!\n");
		printf("Error code: %d\n", GetLastError());
		return 1;
	}

	if (!LookupAccountName(
		NULL,
		username,
		oloSid,
		&sz,
		domain,
		&domainSize,
		&use )) {
		printf("Error when calling \"LookupAccountName()\"!\n");
		printf("Error code: %d\n", GetLastError());
		return 1;
	}

	if (!SetSecurityDescriptorOwner(securityAttributes.lpSecurityDescriptor, oloSid, false)) {
		printf("Error when calling \"SetSecurityDescriptorOwner() for owner\"!\n");
		return 1;
	}

	if (!SetSecurityDescriptorGroup(securityAttributes.lpSecurityDescriptor, ownerSid, false)) {
		printf("Error when calling \"SetSecurityDescriptorGroup() for owner\"!\n");
		return 1;
	}

	if (!SetSecurityDescriptorGroup(securityAttributes.lpSecurityDescriptor, usersSid, false)) {
		printf("Error when calling \"SetSecurityDescriptorGroup() for all users\"!\n");
		return 1;
	}

	const int accessesNum = 3;
	EXPLICIT_ACCESS accesses[accessesNum];
	memset(accesses, 0, sizeof(EXPLICIT_ACCESS) * accessesNum);

	accesses[0].grfAccessPermissions = KEY_READ;
	accesses[0].grfAccessMode = SET_ACCESS;
	accesses[0].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	accesses[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	accesses[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	accesses[0].Trustee.ptstrName = (LPTSTR)usersSid;

	accesses[1].grfAccessPermissions = KEY_ALL_ACCESS;
	accesses[1].grfAccessMode = SET_ACCESS;
	accesses[1].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	accesses[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	accesses[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	accesses[1].Trustee.ptstrName = (LPTSTR)ownerSid;

	accesses[2].grfAccessPermissions = KEY_ALL_ACCESS;
	accesses[2].grfAccessMode = SET_ACCESS;
	accesses[2].grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
	accesses[2].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	accesses[2].Trustee.TrusteeType = TRUSTEE_IS_USER;
	accesses[2].Trustee.ptstrName = (LPTSTR)oloSid;

	PACL pDacl = nullptr;
	if (SetEntriesInAcl(accessesNum, accesses, NULL, &pDacl) != ERROR_SUCCESS) {
		printf("Error when calling \"SetEntriesInAcl()\"!\n");
		printf("Error: %d\n", GetLastError());
		return 1;
	}

	if (!SetSecurityDescriptorDacl(
		securityAttributes.lpSecurityDescriptor,
		true,
		pDacl,
		false)) {
		printf("Error when calling \"SetSecurityDescriptorDacl()\"!\n");
		printf("Error: %d\n", GetLastError());
		return 1;
	}

	DWORD errorCode;
	if ((errorCode = RegCreateKeyEx( 
			registryHKey,
			registrySubPath,
			0, NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			&securityAttributes,
			&hKey,
			&disposition )) != ERROR_SUCCESS ) {
		printf("Error when calling \"RegCreateKeyEx()\"!\n");
		printf("Error: %d\n", GetLastError());
		printf("Error code: %d\n", errorCode);
		return 1;
	}

	if (pDacl) {
		LocalFree(pDacl);
	}
	if (ownerSid) {
		delete[] ownerSid;
	}
	if (usersSid) {
		delete[] usersSid;
	}
	if (oloSid) {
		delete[] oloSid;
	}
	if (domain) {
		delete[] domain;
	}

	if (disposition == (DWORD)REG_OPENED_EXISTING_KEY) {
		printf("The registry key already existed!\n");
		return 1;
	}
	printf("Registry key successfully created! %s\n", (const char*)registrySubPath);

	for (auto regValue : regValuesToCreate) {
		createRegValue(hKey, regValue.first.c_str(), regValue.second);
	}

	RegCloseKey(hKey);

	return 0;
}
