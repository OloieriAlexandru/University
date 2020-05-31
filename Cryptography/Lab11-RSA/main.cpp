#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>

#include <NTL/ZZ.h>

#include "RSA.h"
#include "common.h"

std::vector<std::string>commands;

void initCommandLine() {
	commands.push_back("generate-keys");
	commands.push_back("encrypt");
	commands.push_back("decrypt");
	commands.push_back("decrypt-crt");
	commands.push_back("decryption-experiments");
	commands.push_back("wiener-generate-keys");
	commands.push_back("wiener-attack");
	commands.push_back("list");
	commands.push_back("quit");
}

void printCommands() {
	for (unsigned i = 0; i < commands.size(); ++i) {
		std::cout << i+1 << ". " << commands[i] << '\n';
	}
}

bool stringIsUInt(const std::string& str) {
	if (str.size() > 9) {
		return false;
	}
	for (unsigned i = 0; i < str.size(); ++i) {
		if (!(str[i] >= '0' && str[i] <= '9')) {
			return false;
		}
	}
	return true;
}

int getUIntFromString(const std::string& str) {
	int res = 0;
	for (unsigned i = 0; i < str.size(); ++i) {
		res = res * 10 + (str[i]-'0');
	}
	return res;
}

void trim(std::string& str) {
	reverse(str.begin(), str.end());
	while (str.size() && str.back() == ' ') {
		str.pop_back();
	}
	reverse(str.begin(), str.end());
	while (str.size() && str.back() == ' ') {
		str.pop_back();
	}
}

int parseLine(std::string& line) {
	trim(line);
	if (stringIsUInt(line)) {
		int option = getUIntFromString(line);
		--option;
		if (!(option >= 0 && option < commands.size())) {
			return -1;
		}
		return option;
	}
	for (unsigned i = 0; i < commands.size(); ++i) {
		if (commands[i] == line) {
			return i;
		}
	}
	return -1;
}

void commandGenerateKeys() {
	RSA rsa;
	rsa.generateRandomKeys();
}

void commandEncrypt() {
	RSA rsa;
	
	NTL::ZZ n;
	NTL::ZZ e;
	readBigNumber(n, "Enter n:\n");
	readBigNumber(e, "Enter e:\n");
	std::string message = readStringInput("Enter the message: ");
	
	NTL::ZZ encrypted;
	rsa.encrypt(n, e, message, encrypted);

	std::cout << "Encrypted message: " << encrypted << '\n';
}

void commandDecrypt() {
	RSA rsa;

	NTL::ZZ n;
	NTL::ZZ d;
	NTL::ZZ encrypted;
	readBigNumber(n, "Enter n:\n");
	readBigNumber(d, "Enter d:\n");
	readBigNumber(encrypted, "Enter the encrypted message:\n");

	std::string decrypted;
	decrypted = rsa.decrypt(n, d, encrypted);

	std::cout << "Decrypted message: " << decrypted << '\n';
}

void commandDecryptCRT() {
	RSA rsa;

	NTL::ZZ n;
	NTL::ZZ d;
	NTL::ZZ p;
	NTL::ZZ q;
	NTL::ZZ encrypted;
	readBigNumber(n, "Enter n:\n");
	readBigNumber(d, "Enter d:\n");
	readBigNumber(p, "Enter p:\n");
	readBigNumber(q, "Enter q:\n");
	readBigNumber(encrypted, "Enter the encrypted message:\n");

	std::string decrypted;
	decrypted = rsa.decryptCRT(n, p, q, d, encrypted);

	std::cout << "Decrypted message: " << decrypted << '\n';
}

void decryptionExperiments(const NTL::ZZ& n, const NTL::ZZ& d, const NTL::ZZ& p, 
	const NTL::ZZ q, const NTL::ZZ& e, int numberOfRuns) {
	long long normalDecryptionDuration = 0;
	long long crtDecryptionDuration = 0;
	RSA rsa;
	int match = 0;

	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= numberOfRuns; ++i) {
		std::string toEncryptStr = getRandomString(10);
		NTL::ZZ encryptedBigNumber;
		std::string normalStr;
		std::string crtStr;

		rsa.encrypt(n, e, toEncryptStr, encryptedBigNumber);
		
		start = std::chrono::high_resolution_clock::now();
		normalStr = rsa.decrypt(n, d, encryptedBigNumber);
		end = std::chrono::high_resolution_clock::now();
		normalDecryptionDuration += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		start = std::chrono::high_resolution_clock::now();
		crtStr = rsa.decryptCRT(n, p, q, d, encryptedBigNumber);
		end = std::chrono::high_resolution_clock::now();
		crtDecryptionDuration += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	
		if (normalStr == crtStr) {
			++match;
		}
	}

	std::cout << "Results matched: " << match << "/" << numberOfRuns << '\n';
	std::cout << "Normal decryption total duration: " << normalDecryptionDuration << '\n';
	std::cout << "CRT decryption total duration: " << crtDecryptionDuration << '\n';
}

void commandDecryptionExperiments() {
	NTL::ZZ n;
	NTL::ZZ d;
	NTL::ZZ p;
	NTL::ZZ q;
	NTL::ZZ e;
	readBigNumber(n, "Enter n:\n");
	readBigNumber(d, "Enter d:\n");
	readBigNumber(p, "Enter p:\n");
	readBigNumber(q, "Enter q:\n");
	readBigNumber(e, "Enter e:\n");

	int numberOfRuns = readInt("Enter the number of runs: ");
	decryptionExperiments(n, d, p, q, e, numberOfRuns);
}

void commandGenerateWienerKeys() {
	RSA rsa;
	rsa.generateKeysWiener();
}

void commandWienerAttack() {
	RSA rsa;

	NTL::ZZ n;
	NTL::ZZ e;
	NTL::ZZ d;
	readBigNumber(n, "Enter n:\n");
	readBigNumber(e, "Enter e:\n");
	rsa.wienerAttack(n, e, d);

	std::cout << "Found decryption key d: " << d << '\n';
}

int main() {
	srand(time(NULL));
	initCommandLine();
	printCommands();

	bool running = true;
	while (running){
		std::string line;
		std::getline(std::cin, line);
		int command = parseLine(line);
		switch (command)
		{
		case 0:
			commandGenerateKeys();
			break;
		case 1:
			commandEncrypt();
			break;
		case 2:
			commandDecrypt();
			break;
		case 3:
			commandDecryptCRT();
			break;
		case 4:
			commandDecryptionExperiments();
			break;
		case 5:
			commandGenerateWienerKeys();
			break;
		case 6:
			commandWienerAttack();
			break;
		case 7:
			printCommands();
			break;
		case 8:
			running = false;
			break;
		default:
			std::cout << "Invalid command!\n";
		}
	}
	return 0;
}
