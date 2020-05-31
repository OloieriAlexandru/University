#pragma once

#include <string>
#include <NTL/ZZ.h>
#include <vector>
#include <utility>

#include "common.h"

class RSA {
private:
	void generatePrimeNumber(NTL::ZZ& num, int bits);

	std::string decryptCRTArgs(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& d, const NTL::ZZ& num);

	std::vector<NTL::ZZ> getTheContinuedFractionExpansion(const NTL::ZZ& cn, const NTL::ZZ& ce);

	std::vector<std::pair<NTL::ZZ, NTL::ZZ>> getTheConvergents(const std::vector<NTL::ZZ>& continuedFractionExpansion);
public:
	void generateRandomKeys(bool save = false);
	
	void generateKeysWiener();

	void encrypt(const NTL::ZZ& n, const NTL::ZZ& e, const std::string& str, NTL::ZZ& encrypted);

	std::string decrypt(const NTL::ZZ& n, const NTL::ZZ& d, const NTL::ZZ& num);

	std::string decryptCRT(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& d, const NTL::ZZ& num);

	// https://www.infsec.cs.uni-saarland.de/teaching/16WS/Cybersecurity/lecture_notes/cysec16-ln05.pdf
	// https://profs.info.uaic.ro/~siftene/Wiener.pdf
	void wienerAttack(const NTL::ZZ& n, const NTL::ZZ& e, NTL::ZZ& d);
};
