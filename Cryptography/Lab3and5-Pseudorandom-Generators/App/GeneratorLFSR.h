#pragma once

#include "Generator.h"

class GeneratorLFSR : public Generator {
private:
	virtual void loadPrimesFromFile() override;

	static const int L = 229;
	static const int stages[2];
	std::vector<std::list<bool>::iterator> xorStages;

	std::list<bool> state;

	void updateState();
public:
	GeneratorLFSR();

	virtual NTL::ZZ generate(bool runTests = false) override;

	virtual const char* getName() override;

	virtual void generateBytes(int numberOfBytes) override;
};