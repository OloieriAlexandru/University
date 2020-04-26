#include "GeneratorJacobi.h"

GeneratorJacobi::GeneratorJacobi() {
	generatePrimeNumbers();
	loadPrimesFromFile();
	x0 = time(0);
}

void GeneratorJacobi::loadPrimesFromFile() {
	std::ifstream fileIn(fileName);
	fileIn >> n;
	fileIn.close();
}

int GeneratorJacobi::computeJacobiSymbol(const NTL::ZZ& constA, const NTL::ZZ& constN) {
	NTL::ZZ a = constA;
	NTL::ZZ n = constN;
	int t = 1, r, r1, r2;
		
	while (a != 0) {
		while (a % 2 == 0) {
			a /= 2;
			r = n % 8;
			if (r == 3 || r == 5) {
				t = -t;
			}
		}
		NTL::ZZ cp = n;
		n = a;
		a = cp;
		r1 = a % 4;
		r2 = n % 4;
		if (r1 == 3 && r2 == 3) {
			t = -t;
		}
		a %= n;
	}

	if (n == 1) {
		return t;
	}
	return 0;
}

NTL::ZZ GeneratorJacobi::generate(bool runTests) {
	NTL::ZZ res(0);
	int n0 = 0, jacobi;
	std::string b2;

	for (int i = 1; i <= generatedNumbersLen; ++i, ++x0) {
		jacobi = computeJacobiSymbol(x0, n);
		if (jacobi == -1) {
			jacobi = 0;
		}
		if (!jacobi) {
			++n0;
			b2.push_back('0');
		} else {
			b2.push_back('1');
		}
		res = res * 2 + jacobi;
	}

	if (runTests) {
		tests(n0, b2, "Jacobi Generator");
	}
	return res;
}

const char* GeneratorJacobi::getName() {
	return "Jacobi Generator";
}

void GeneratorJacobi::checkJacobiSymbolCalculation() {
	std::vector<std::pair<std::pair<int, int>, int>> testData;

	testData.push_back(std::make_pair(std::make_pair(1235, 20003), -1));
	testData.push_back(std::make_pair(std::make_pair(1236, 20003), 1));
	testData.push_back(std::make_pair(std::make_pair(45, 131), 1));
	testData.push_back(std::make_pair(std::make_pair(46, 131), 1));
	testData.push_back(std::make_pair(std::make_pair(47, 131), -1));
	testData.push_back(std::make_pair(std::make_pair(123456, 1000007), -1));

	for (auto test : testData) {
		NTL::ZZ m(test.first.first);
		NTL::ZZ n(test.first.second);
		int expectedResult = test.second;

		int jacobi = computeJacobiSymbol(m, n);
		
		if (jacobi == expectedResult) {
			std::cout << "GOOD";
		} else {
			std::cout << " BAD";
		}
		std::cout << ", m = " << m << ", n = " << n << ", expected result: " << expectedResult << ", computed symbol: " << jacobi << '\n';
	}
}

void GeneratorJacobi::generateBytes(int numberOfBytes) {
	int jacobi;
	for (int i = 1; i <= generatedNumbersLen; ++i, ++x0) {
		jacobi = computeJacobiSymbol(x0, n);
		if (jacobi == -1) {
			jacobi = 0;
		}
		std::cout << jacobi;
	}
	std::cout << '\n';
}
