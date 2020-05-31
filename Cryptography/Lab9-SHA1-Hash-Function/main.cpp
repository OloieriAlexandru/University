#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>
#include <utility>
#include <iomanip>
#include <unordered_map>

#include "SHA1.h"
#include "common.h"

#define uc unsigned char

std::vector<std::string>commands;

void initCommandLine() {
	commands.push_back("sha1");
	commands.push_back("sha1-32");
	commands.push_back("validate");
	commands.push_back("avalanche");
	commands.push_back("birthday-attack");
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

void printHash(const std::vector<uc>& binaryHash) {
	std::string hexHash = binaryToHexTransf(binaryHash);

	std::cout << "Hash:\n";
	std::cout << "binary: ";
	printBinaryArr(binaryHash);
	std::cout << "hex: " << hexHash << '\n';
}

void sha1Command() {
	SHA1 sha1;

	std::string str = readStringInput("Enter the text: ");

	std::vector<uc> binaryHash = sha1.hash(str);
	printHash(binaryHash);
}

void sha1Command32() {
	SHA1 sha1;

	std::string str = readStringInput("Enter the text: ");

	std::vector<uc> binaryHash = sha1.truncHashBin(str, 32);
	printHash(binaryHash);
}

void validateSHA() {
	SHA1 sha1;
	std::vector<std::pair<std::string, std::string>> test;

	test.push_back(std::make_pair("abc", "a9993e364706816aba3e25717850c26c9cd0d89d"));
	test.push_back(std::make_pair("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "84983e441c3bd26ebaae4aa1f95129e5e54670f1"));
	test.push_back(std::make_pair("", "da39a3ee5e6b4b0d3255bfef95601890afd80709"));
	test.push_back(std::make_pair("a", "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8"));
	test.push_back(std::make_pair("abc", "a9993e364706816aba3e25717850c26c9cd0d89d"));
	test.push_back(std::make_pair("abcdefghijklmnopqrstuvwxyz", "32d10c7b8cf96570ca04ce37f2a19d84240d3a89"));

	for (auto x : test) {
		std::string hexHash = binaryToHexTransf(sha1.hash(x.first));
		
		if (hexHash == x.second) {
			std::cout << "GOOD, ";
		} else {
			std::cout << " BAD, ";
		}
		std::cout << "text: " << x.first << ", expected: " << x.second << ", hash: " << hexHash << '\n';
	}
}

int hammingDistance(const std::vector<uc>& a, const std::vector<uc>& b) {
	int res = 0;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != b[i]) {
			++res;
		}
	}
	return res;
}

void checkAvalanche() {
	SHA1 sha1;

	std::string str = readStringInput("Enter some text: ");

	std::vector<uc> binaryStr = sha1.getAsBinary(str);
	printBinaryArr(binaryStr);
	std::vector<uc> binaryHash1 = sha1.hash(str);
	printHash(binaryHash1);

	str[str.size() - 1] ^= 1;
	std::vector<uc> binaryStr2 = sha1.getAsBinary(str);
	printBinaryArr(binaryStr2);
	std::vector<uc>binaryHash2 = sha1.hash(str);
	printHash(binaryHash2);

	std::cout << "Hamming distance: " << hammingDistance(binaryHash1, binaryHash2) << '\n';
}

// https://en.wikipedia.org/wiki/Birthday_attack#Mathematics
void birthdayAttack() {
	SHA1 sha1;

	std::unordered_map<uint, int> hashToKey;
	const int toGenerate = 110000;
	int start = (1LL * rand() * rand()) % (1 << 27);
	int to = start + toGenerate;
	int found = 0;
	std::cout << "Generating " << toGenerate << " consecutive numbers starting with " << start << '\n';
	for (int i = start; i < to; ++i) {
		uint hsh = sha1.truncHash(i, 32);
		
		if (hashToKey.count(hsh)) {
			std::cout << "Collision found! text1: " << hashToKey[hsh] << ", text2: " << i << '\n';
			++found;
		}

		hashToKey[hsh] = i;
	}
	if (found) {
		std::cout << found << " colisions found after generating " << toGenerate << " texts!\n";
	} else {
		std::cout << "No collision found after generating " << toGenerate << " texts!\n";
	}
}

int main() {
	srand(time(NULL));

	initBinaryToHexMap();
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
			sha1Command();
			break;
		case 1:
			sha1Command32();
			break;
		case 2:
			validateSHA();
			break;
		case 3:
			checkAvalanche();
			break;
		case 4:
			birthdayAttack();
			break;
		case 5:
			printCommands();
			break;
		case 6:
			running = false;
			break;
		default:
			std::cout << "Invalid command!\n";
			break;
		}
	}
	return 0;
}