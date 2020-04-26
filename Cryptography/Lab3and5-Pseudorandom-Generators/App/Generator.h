#pragma once

#include <fstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <cstdlib>
#include <map>

#include <NTL/ZZ.h>

class Generator {
private:
	std::string oneString;

	void compressionTest(const std::string& b2);
protected:
	Generator();

	int generatedNumbersLen = 1 << 10;

	static const char* fileName;
	
	bool primeNumbersFileIsCreated();
	void generatePrimeNumbers();

	void tests(int n0, const std::string& b2, const char* generatorName);

	virtual void loadPrimesFromFile() = 0;
public:
	virtual NTL::ZZ generate(bool runTests = false) = 0;

	virtual void generateBytes(int numberOfBytes) = 0;

	virtual const char* getName() = 0;
};