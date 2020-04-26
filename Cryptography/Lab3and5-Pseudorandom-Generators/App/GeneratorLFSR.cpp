#include "GeneratorLFSR.h"

const int GeneratorLFSR::stages[2] = { 64, 229 };

void GeneratorLFSR::loadPrimesFromFile() { }

void GeneratorLFSR::updateState() {
	bool newBit = 0;
	for (int i = 0; i < 2; ++i) {
		newBit ^= *(xorStages[i]);
	}
	state.push_back(newBit);
	for (int i = 0; i < 2; ++i) {
		++xorStages[i];
	}
	state.pop_front();
}

GeneratorLFSR::GeneratorLFSR() {
	int zero = 0;
	for (int i = 0; i < L; ++i) {
		if (rand() % 2) {
			state.push_back(true);
		} else {
			state.push_back(false);
			++zero;
		}
		for (int j = 0; j < 2; ++j) {
			if ((L - stages[j]) == i) {
				auto it = state.end();
				--it;
				xorStages.push_back(it);
			}
		}
	}
	if (!zero) {
		*(state.begin()) = true;
	} else if (zero == L) {
		*(state.begin()) = false;
	}
}

NTL::ZZ GeneratorLFSR::generate(bool runTests) {
	NTL::ZZ res(0);
	int n0 = 0;
	std::string b2;

	for (int i = 1; i <= generatedNumbersLen; ++i) {
		int add = *(state.begin());
		if (!add) {
			++n0;
			b2.push_back('0');
		} else {
			b2.push_back('1');
		}
		updateState();
		res = res * 2 + add;
	}

	if (runTests) {
		tests(n0, b2, "LFSR Generator");
	}
	return res;
}

const char* GeneratorLFSR::getName() {
	return "LFSR Generator";
}

void GeneratorLFSR::generateBytes(int numberOfBytes) {
	for (int i = 1; i <= numberOfBytes; ++i) {
		int add = *(state.begin());
		updateState();
		std::cout << add;
	}
	std::cout << '\n';
}
