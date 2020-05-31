#include "common.h"

std::string readStringInput(const std::string& message) {
	std::string line;

	std::cout << message;
	std::getline(std::cin, line);

	return line;
}

void initBinaryToHexMap() {
	binaryToHex[std::string("0000")] = '0';
	binaryToHex[std::string("0001")] = '1';
	binaryToHex[std::string("0010")] = '2';
	binaryToHex[std::string("0011")] = '3';
	binaryToHex[std::string("0100")] = '4';
	binaryToHex[std::string("0101")] = '5';
	binaryToHex[std::string("0110")] = '6';
	binaryToHex[std::string("0111")] = '7';
	binaryToHex[std::string("1000")] = '8';
	binaryToHex[std::string("1001")] = '9';
	binaryToHex[std::string("1010")] = 'a';
	binaryToHex[std::string("1011")] = 'b';
	binaryToHex[std::string("1100")] = 'c';
	binaryToHex[std::string("1101")] = 'd';
	binaryToHex[std::string("1110")] = 'e';
	binaryToHex[std::string("1111")] = 'f';
}

std::string uintToBinary(uint x) {
	std::vector<uc> arr;
	std::bitset<32> bs(x);
	for (int i = 31; i >= 0; --i) {
		arr.push_back(bs.test(i));
	}
	return binaryToHexTransf(arr);
}

std::string binaryToHexTransf(const std::vector<uc>& arr) {
	std::string res;

	for (int i = 0; i < arr.size(); i += 4) {
		std::string str;
		for (int j = 0; j < 4; ++j) {
			str.push_back(arr[i + j] + '0');
		}
		res.push_back(binaryToHex[str]);
	}

	return res;
}

void printBinaryArr(const std::vector<uc>& arr) {
	for (auto x : arr) {
		std::cout << (int)x;
	}
	std::cout << '\n';
}
