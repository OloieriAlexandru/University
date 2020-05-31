#include "RSA.h"

void RSA::generatePrimeNumber(NTL::ZZ& num, int bits) {
	NTL::GenGermainPrime(num, bits, 80);
}

std::string RSA::decryptCRTArgs(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& d, const NTL::ZZ& num) {
	std::string ans;
	NTL::ZZ n1 = d % (p - 1);
	NTL::ZZ n2 = d % (q - 1);
	NTL::ZZ inv = NTL::InvMod(p, q);

	NTL::ZZ x1;
	NTL::PowerMod(x1, num % p, n1, p);

	NTL::ZZ x2;
	NTL::PowerMod(x2, num % q, n2, q);

	NTL::ZZ mul = (x2 - x1) * (inv % q) % q;
	if (mul < 0) {
		mul += q;
	}
	NTL::ZZ numAns = x1 + p * mul;
	bigNumberToString(numAns, ans);
	return ans;
}

std::vector<NTL::ZZ> RSA::getTheContinuedFractionExpansion(const NTL::ZZ& cn, const NTL::ZZ& ce) {
	std::vector<NTL::ZZ> fractionExpansion;

	NTL::ZZ n = cn;
	NTL::ZZ e = ce;
	NTL::ZZ r;
	while (n != 0) {
		fractionExpansion.push_back(e / n);
		r = e % n;
		e = n;
		n = r;
	}

	return fractionExpansion;
}

std::vector<std::pair<NTL::ZZ, NTL::ZZ>> RSA::getTheConvergents(const std::vector<NTL::ZZ>& continuedFractionExpansion) {
std::vector<std::pair<NTL::ZZ, NTL::ZZ>> convergents;
NTL::ZZ alpha = continuedFractionExpansion[0];
NTL::ZZ beta(1);

convergents.push_back(std::make_pair(alpha, beta));
alpha = continuedFractionExpansion[0] * continuedFractionExpansion[1] + 1;
beta = continuedFractionExpansion[1];

convergents.push_back(std::make_pair(alpha, beta));
for (int i = 2; i < continuedFractionExpansion.size(); ++i) {
	alpha = continuedFractionExpansion[i] * convergents[i - 1].first + convergents[i - 2].first;
	beta = continuedFractionExpansion[i] * convergents[i - 1].second + convergents[i - 2].second;
	convergents.push_back(std::make_pair(alpha, beta));
}

return convergents;
}

void RSA::generateRandomKeys(bool save) {
	NTL::ZZ p;
	NTL::ZZ q;
	generatePrimeNumber(p, 512);
	generatePrimeNumber(q, 512);

	NTL::ZZ n = p * q;
	NTL::ZZ phi = (p - 1) * (q - 1);

	NTL::ZZ e(65537);
	NTL::ZZ d;
	NTL::InvMod(d, e, phi);

	printRSAVariables(n, p, q, e, d);
}

void RSA::generateKeysWiener() {
	NTL::ZZ p;
	NTL::ZZ q;
	while (true) {
		generatePrimeNumber(p, 512);
		generatePrimeNumber(q, 512);
		if (p < q) {
			NTL::swap(p, q);
		}
		if (q < p && p < 2 * q) {
			break;
		}
	}

	NTL::ZZ n = p * q;
	NTL::ZZ phi = (p - 1) * (q - 1);

	NTL::ZZ d;
	while (true) {
		generatePrimeNumber(d, 32);
		if (NTL::power(d, 3) * 3 < n) {
			break;
		}
	}
	NTL::ZZ e;
	NTL::InvMod(e, d, phi);

	printRSAVariables(n, p, q, e, d);
}

void RSA::encrypt(const NTL::ZZ& n, const NTL::ZZ& e, const std::string& str, NTL::ZZ& encrypted) {
	NTL::ZZ numStr;
	stringToBigNumber(str, numStr);

	NTL::PowerMod(encrypted, numStr, e, n);
}

std::string RSA::decrypt(const NTL::ZZ& n, const NTL::ZZ& d, const NTL::ZZ& num) {
	std::string ans;
	NTL::ZZ numAns;

	NTL::PowerMod(numAns, num, d, n);

	bigNumberToString(numAns, ans);
	return ans;
}

// https://profs.info.uaic.ro/~tr/tr03-02.pdf pagina 69, 7.4
std::string RSA::decryptCRT(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& d, const NTL::ZZ& num) {
	if (p < q) {
		return decryptCRTArgs(n, p, q, d, num);
	}
	return decryptCRTArgs(n, q, p, d, num);
}

void RSA::wienerAttack(const NTL::ZZ& n, const NTL::ZZ& e, NTL::ZZ& d) {
	std::vector<NTL::ZZ> fractionExpansion = getTheContinuedFractionExpansion(n, e);
	std::vector<std::pair<NTL::ZZ, NTL::ZZ>> convergents = getTheConvergents(fractionExpansion);

	d = 0;

	for (auto x : convergents) {
		if (x.first == 0) {
			continue;
		}

		if ((e * x.second - 1) % x.first != 0){
			continue;
		}
		NTL::ZZ phiPos = (e * x.second - 1) / x.first;
		NTL::ZZ a(1);
		NTL::ZZ b = (n - phiPos + 1) * -1;
		NTL::ZZ c = n;

		NTL::ZZ delta = b * b - 4 * a * c;
		if (delta <= 0) {
			continue;
		}
		NTL::ZZ vSqrt;
		NTL::SqrRoot(vSqrt, delta);
		if (vSqrt * vSqrt != delta) {
			continue;
		}

		b = b * -1;
		NTL::ZZ x1 = (b + vSqrt) / (2 * a);
		NTL::ZZ x2 = (b - vSqrt) / (2 * a);

		if (x1 * x2 == n) {
			NTL::InvMod(d, e, phiPos);
		}
	}
}
