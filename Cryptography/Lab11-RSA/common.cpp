#include "common.h"

int readInt(const std::string& message) {
	std::string line;
	std::cout << message;
	std::getline(std::cin, line);
	int num = 0;
	for (int i = 0; i < line.size(); ++i) {
		num = num * 10 + (line[i] - '0');
	}
	return num;
}

void readBigNumber(NTL::ZZ& num) {
	std::string line;
	std::getline(std::cin, line);
	num = 0;
	for (int i = 0; i < line.size(); ++i) {
		num = num * 10 + (line[i] - '0');
	}
}

void readBigNumber(NTL::ZZ& num, const std::string& message) {
	std::cout << message;
	readBigNumber(num);
}

void stringToBigNumber(const std::string& str, NTL::ZZ& num) {
	num = 0;
	for (int i = 0; i < str.size(); ++i) {
		for (int j = 7; j >= 0; --j) {
			if ((1 << j) & str[i]) {
				num = num * 2 + 1;
			} else {
				num *= 2;
			}
		}
	}
}

void bigNumberToString(const NTL::ZZ& num, std::string& str) {
	NTL::ZZ copyNum = num;
	str.clear();

	while (copyNum > 0) {
		char chr = 0;
		int p2 = 1;
		for (int i = 0; i < 8; ++i) {
			chr = chr + (copyNum % 2) * p2;
			copyNum /= 2;
			p2 *= 2;
		}
		str.push_back(chr);
	}
	
	std::reverse(str.begin(), str.end());
}

std::string readStringInput(const std::string& message) {
	std::string line;

	std::cout << message;
	std::getline(std::cin, line);

	return line;
}

std::string getRandomString(int len) {
	std::string str;
	str.resize(len);

	for (int i = 0; i < len; ++i) {
		str[i] = (rand() % 256);
	}

	return str;
}

void printRSAVariables(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& e, const NTL::ZZ& d) {
	std::cout << "n:\n" << n << '\n';
	std::cout << "e:\n" << e << '\n';
	std::cout << "d:\n" << d << '\n';
	std::cout << "p:\n" << p << '\n';
	std::cout << "q:\n" << q << '\n';
}
