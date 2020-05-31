#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <map>

#include "common.h"
#include "DES.h"

std::vector<std::string>commands;

void initCommandLine() {
	commands.push_back("encrypt");
	commands.push_back("decrypt");
	commands.push_back("meet-in-the-middle-attack");
	commands.push_back("list");
	commands.push_back("quit");
}

void printCommands() {
	for (unsigned i = 0; i < commands.size(); ++i) {
		std::cout << i + 1 << ". " << commands[i] << '\n';
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
		res = res * 10 + (str[i] - '0');
	}
	return res;
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

void commandEncrypt() {
	DES des;
	
	std::vector<uc> text = readText();
	std::vector<uc> key = readKey(8);

	std::vector<uc> encryptedText = des.encrypt(text, key);

	std::cout << "Encryption complete!\n";
	printInAllForms(encryptedText);
}

void commandDecrypt() {
	DES des;

	std::vector<uc> text = readText();
	std::vector<uc> key = readKey(8);

	std::vector<uc> decryptedText = des.decrypt(text, key);

	std::cout << "Decryption complete!\n";
	printInAllForms(decryptedText);
}

std::vector<uc> doubleDES(const std::vector<uc>& plaintext, const std::vector<uc>& key1, const std::vector<uc>& key2) {
	DES des;

	std::vector<uc> ciphertext = des.encrypt(plaintext, key1);
	return des.encrypt(ciphertext, key2);
}

std::vector<uc> byteX8(const std::vector<uc>& byte) {
	return multiply(binaryToInt(byte), 8);
}

std::vector<uc> getKeyFromByte(const std::vector<uc>& byte) {
	if (true) {
		return byteX8(byte);
	}
	return multiply(byte, 8);
}

std::vector<uc> getKeyFromUChar(uc c) {
	return getKeyFromByte(uCharToBinary(c));
}

bool checkSecondPair(const std::vector<uc>& plaintext, const std::vector<uc>& expectedCiphertext, uc key1, uc key2) {
	std::vector<uc> k1 = getKeyFromUChar(key1);
	std::vector<uc> k2 = getKeyFromUChar(key2);
	DES des;

	std::vector<uc> ciphertext = des.encrypt(plaintext, k1);
	ciphertext = des.encrypt(ciphertext, k2);

	return ciphertext == expectedCiphertext;
}

void meetInTheMiddleAttack(const std::vector<uc>& plaintext, const std::vector<uc>& ciphertext,
	const std::vector<uc>& secondPlaintext, const std::vector<uc>& secondCiphertext) {
	DES des;
	std::map<std::vector<uc>, uc> mp;

	for (int i = 0; i < 256; ++i) {
		std::vector<uc> key = getKeyFromUChar(i);

		std::vector<uc> encrypted = des.encrypt(plaintext, key);
		
		mp[encrypted] = (uc)i;
	}
	
	for (int i = 0; i < 256; ++i) {
		std::vector<uc> key = getKeyFromUChar(i);

		std::vector<uc> decrypted = des.decrypt(ciphertext, key);

		if (mp.count(decrypted)) {
			if (!checkSecondPair(secondPlaintext, secondCiphertext, mp[decrypted], i)) {
				continue;
			}
			std::cout << "Solution found: \n";
			int k1 = mp[decrypted];
			std::cout << "key1 = (byte)" << (int)k1 << ", (ascii)" << (char)k1 << '\n';
			std::cout << "key2 = (byte)" << (int)i << ", (ascii)" << (char)i << '\n';
		}
	}
}

void commandMeetInTheMiddleAttack() {
	std::cout << "t1: Enter the plaintext and two 1 byte keys\n";
	std::cout << "t2: Enter the plaintext and the double encrypted ciphertext\n";
	std::string type = readOption("Enter the type of the attack: ", std::vector<std::string> {"t1", "t2"});

	std::vector<uc> plaintext, ciphertext;
	std::vector<uc> sPlaintext, sCiphertext;

	if (type == "t1") {
		std::cout << "Enter the plaintext:\n";
		plaintext = readText();
		std::cout << "Enter the second plaintext:\n";
		sPlaintext = readText();
		std::cout << "Enter the first key (1 byte/ 8 bits):\n";
		std::vector<uc> key1 = readKey(1);
		std::cout << "Enter the second key (1 byte/ 8 bits):\n";
		std::vector<uc> key2 = readKey(1);

		DES des;
		
		ciphertext = des.encrypt(plaintext, getKeyFromByte(key1));
		ciphertext = des.encrypt(ciphertext, getKeyFromByte(key2));

		sCiphertext = des.encrypt(sPlaintext, getKeyFromByte(key1));
		sCiphertext = des.encrypt(sCiphertext, getKeyFromByte(key2));
	} else {
		std::cout << "Enter the plaintext:\n";
		plaintext = readText();
		std::cout << "Enter the ciphertext:\n";
		ciphertext = readText();
		std::cout << "Enter the second plaintext:\n";
		sPlaintext = readText();
		std::cout << "Enter the second ciphertext:\n";
		sCiphertext = readText();
	}

	meetInTheMiddleAttack(plaintext, ciphertext, sPlaintext, sCiphertext);
}

int main() {
	srand(time(NULL));
	initBinaryToHexMap();
	initHexToBinaryMap();

	initCommandLine();
	printCommands();

	bool running = true;
	while (running) {
		std::string line;
		std::getline(std::cin, line);
		if (line.size() == 0) {
			continue;
		}
		int command = parseLine(line);
		switch (command) {
		case 0:
			commandEncrypt();
			break;
		case 1:
			commandDecrypt();
			break;
		case 2:
			commandMeetInTheMiddleAttack();
			break;
		case 3:
			printCommands();
			break;
		case 4:
			running = false;
			break;
		default:
			std::cout << "Invalid command!\n";
			break;
		}
	}
	return 0;
}
