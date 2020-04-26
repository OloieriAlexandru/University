#pragma once

#include "Generator.h"

class GeneratorBBS : public Generator {
private:
	NTL::ZZ n;
	NTL::ZZ x0;

	virtual void loadPrimesFromFile() override;
public:
	GeneratorBBS();

	virtual NTL::ZZ generate(bool runTests = false) override;

	virtual const char* getName() override;

	virtual void generateBytes(int numberOfBytes) override;
};