#include "GeneratorBBS.h"

GeneratorBBS::GeneratorBBS() {
	generatePrimeNumbers();
	loadPrimesFromFile();
	x0 = time(0);
	for (int i = 1; i <= 3; ++i) {
		x0 = (x0 * x0) % n;
	}
}

void GeneratorBBS::loadPrimesFromFile() {
	std::ifstream fileIn(fileName);
	fileIn >> n;
	fileIn.close();
}

NTL::ZZ GeneratorBBS::generate(bool runTests) {
	NTL::ZZ res(0);
	int r, n0 = 0;
	std::string b2;

	for (int i = 1; i <= generatedNumbersLen; ++i) {
		x0 = (x0 * x0) % n;
		r = x0 % 2;
		if (!r) {
			++n0;
			b2.push_back('0');
		} else {
			b2.push_back('1');
		}
		res = res * 2 + r;
	}

	if (runTests) {
		tests(n0, b2, "BBS Generator");
	}
	return res;
}

const char* GeneratorBBS::getName() {
	return "BBS Generator";
}

void GeneratorBBS::generateBytes(int numberOfBytes) {
	int r;
	for (int i = 1; i <= generatedNumbersLen; ++i) {
		x0 = (x0 * x0) % n;
		r = x0 % 2;
		std::cout << r;
	}
	std::cout << '\n';
}
