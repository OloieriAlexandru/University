#include "Generator.h"

const char* Generator::fileName = "primes.txt";

// https://gist.github.com/jacky860226/c436c68beb07782ba91306544a392d9e
std::vector<short> encode(const std::string& ori) {
	std::map<std::string, short> dict;
	int dictSize = 256;
	for (int i = 0; i < dictSize; ++i)
		dict[std::string(1, i)] = i;
	std::vector<short> res;
	std::string s;
	for (char z : ori) {
		if (dict.count(s + z)) s += z;
		else {
			res.emplace_back(dict[s]);
			dict[s + z] = dictSize++;
			s = z;
		}
	}
	if (s.size()) res.emplace_back(dict[s]);
	return res;
}

void Generator::compressionTest(const std::string& b2) {
	std::vector<short> compressed = encode(b2);
	std::cout << "Compressed number: " << compressed.size() * 2 << " bytes, compressed \"111..111\" sequence: ";
	compressed = encode(oneString);
	std::cout << compressed.size() * 2<< " bytes\n";
}

Generator::Generator() {
	for (int i = 0; i < generatedNumbersLen; ++i) {
		oneString.push_back('1');
	}
}

bool Generator::primeNumbersFileIsCreated() {
	std::ifstream fileIn(fileName);
	if (!fileIn.is_open()) {
		return false;
	}
	return true;
}

void Generator::generatePrimeNumbers() {
	if (primeNumbersFileIsCreated()) {
		return;
	}
	std::ofstream fileOut(fileName);

	NTL::ZZ p = NTL::GenPrime_ZZ(512);
	while (p % 4 != 3) {
		p = NTL::GenPrime_ZZ(512);
	}

	NTL::ZZ q = NTL::GenPrime_ZZ(512);
	while (q % 4 != 3) {
		q = NTL::GenPrime_ZZ(512);
	}

	NTL::ZZ n = p * q;
	fileOut << n << '\n';

	fileOut.close();
}

void Generator::tests(int n0, const std::string& b2, const char* generatorName) {
	if (generatorName) {
		std::cout << generatorName << '\n';
	}

	double p0 = ((n0 * 1.0) / generatedNumbersLen) * 100.0;
	std::cout << "Test 1\n";
	std::cout << std::setprecision(3) << std::fixed << "0%: " << p0 << ", 1%: " << (100.0 - p0) << '\n';

	std::cout << "Test 2\n";
	compressionTest(b2);

	std::cout << "Generated number:\n";
}
