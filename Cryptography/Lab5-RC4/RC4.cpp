#include "RC4.h"

void RC4::ksa() {
	for (int i = 0; i < 256; ++i) {
		state.push_back(i);
	}
	j = 0;
	for (int i = 0; i < 256; ++i) {
		j = j + state[i] + key[i % key.size()];
		std::swap(state[i], state[j]);
	}
	i = j = 0;
}

unsigned char RC4::updateInternalState() {
	++i;
	j += state[i];
	std::swap(state[i], state[j]);
	unsigned char byteIndexToReturn = ( state[i] + state[j] ) % 256;
	return state[byteIndexToReturn];
}

void RC4::readKey() {
	int len;
	while (true) {
		std::cout << "Enter key len: ";
		std::cin >> len;
		if (len <= 0) {
			std::cout << "The key len has to be a positive integer!\n";
			continue;
		}
		if (len >= 256) {
			std::cout << "The key has to have at most 255 bytes!\n";
		}
		break;
	}
	for (int i = 1; i <= len; ++i) {
		int intNum;
		std::cin >> intNum;
		unsigned char byte = (unsigned char)intNum;
		key.push_back(byte);
	}
	ksa();
}

void RC4::generateKey() {
	int keyLen = rand() % 28 + 5;
	for (int i = 0; i < keyLen; ++i) {
		unsigned char byte = rand() % 256;
		key.push_back(byte);
	}
	ksa();
}

void RC4::encrypt(const std::vector<unsigned char>& str) {
	for (auto x : str) {
		std::cout << (x ^ updateInternalState()) << ' ';
	}
	std::cout << '\n';
}

void RC4::decrypt(const std::vector<unsigned char>& str) {
	for (auto x : str) {
		std::cout << (char)(x ^ updateInternalState());
	}
	std::cout << '\n';
}

std::vector<unsigned char> RC4::generate(int numberOfBytes) {
	std::vector<unsigned char> res;
	for (int i = 1; i <= numberOfBytes; ++i) {
		res.push_back(updateInternalState());
	}
	return res;
}
