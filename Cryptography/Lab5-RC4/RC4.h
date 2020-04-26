#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class RC4 {
private:
	std::vector<unsigned char> key;
	std::vector<unsigned char> state;
	unsigned char i = 0, j = 0;

	void ksa();
	unsigned char updateInternalState();
public:
	void readKey();
	void generateKey();

	void encrypt(const std::vector<unsigned char>& str);
	void decrypt(const std::vector<unsigned char>& str);

	std::vector<unsigned char> generate(int numberOfBytes);
};