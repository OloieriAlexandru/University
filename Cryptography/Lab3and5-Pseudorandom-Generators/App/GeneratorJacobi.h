#pragma once

#include "Generator.h"

#include <utility>

class GeneratorJacobi : public Generator {
private: 
	NTL::ZZ n;
	NTL::ZZ x0;

	virtual void loadPrimesFromFile() override;

	int computeJacobiSymbol(const NTL::ZZ& a, const NTL::ZZ& n);
public:
	GeneratorJacobi();

	virtual NTL::ZZ generate(bool runTests = false) override;

	virtual const char* getName() override;

	void checkJacobiSymbolCalculation();

	virtual void generateBytes(int numberOfBytes) override;
};