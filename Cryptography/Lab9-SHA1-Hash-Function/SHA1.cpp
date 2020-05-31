#include "SHA1.h"

const uint SHA1::y[4] = { 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6 };

std::vector<uc> SHA1::getAsBinary(const std::string& str) {
	std::vector<uc> res;

	for (auto x : str) {
		std::bitset<8> bs(x);
		for (int j = 7; j >= 0; --j) {
			res.push_back(bs.test(j));
		}
	}

	return res;
}

std::vector<uc> SHA1::pad(const std::string& str) {
	std::vector<uc> bin = getAsBinary(str);
	ull originalLen = bin.size();

	bin.push_back(1);
	while (bin.size() % 512 != 448) {
		bin.push_back(0);
	}
	std::bitset<64> bs(originalLen);
	for (int i = 63; i >= 0; --i) {
		bin.push_back(bs.test(i));
	}

	return bin;
}

std::vector<uc> SHA1::appendBytes(const std::vector<uc>& arr, uint x) {
	std::vector<uc> res = arr;

	std::bitset<32> bs(x);
	for (int i = 31; i >= 0; --i) {
		res.push_back(bs.test(i));
	}

	return res;
}

std::vector<uint> SHA1::getChunkWords(const std::vector<uc>& bin, int chunk) {
	std::vector<uint> res;

	int add = chunk * 512;
	for (int i = 0; i < 16; ++i) {
		uint w = 0;
		for (int j = 0; j < 32; ++j) {
			w = w * 2 + bin[add + i * 32 + j];
		}
		res.push_back(w);
	}

	return res;
}

uint SHA1::rotate(uint x, int pos) {
	uint y = (x >> (32 - pos));
	return (x << pos) | y;
}

uint SHA1::funF(uint a, uint b, uint c) {
	return (a & b) | ((~a) & c);
}

uint SHA1::funG(uint a, uint b, uint c) {
	return (a & b) | (a & c) | (b & c);
}

uint SHA1::funH(uint a, uint b, uint c) {
	return a ^ b ^ c;
}

void SHA1::printWorkingVariables(uint a, uint b, uint c, uint d, uint e) {
	std::cout << uintToBinary(a) << ' ' << uintToBinary(b) << ' ' << uintToBinary(c) << ' ' << uintToBinary(d) << ' ' << uintToBinary(e) << '\n';
}

std::vector<uc> SHA1::hash(const std::string& str) {
	std::vector<uc> bin = pad(str);

	uint h1 = this->h1;
	uint h2 = this->h2;
	uint h3 = this->h3;
	uint h4 = this->h4;
	uint h5 = this->h5;

	int chunksSize = bin.size() / 512;
	for (int i = 0; i < chunksSize; ++i) {
		std::vector<uint> w = getChunkWords(bin, i);

		for (int j = 16; j < 80; ++j) {
			w.push_back(rotate(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1));
		}

		uint a = h1;
		uint b = h2;
		uint c = h3;
		uint d = h4;
		uint e = h5;
		// printWorkingVariables(a, b, c, d, e);

		for (int j = 0; j < 80; ++j) {
			int round = j / 20;

			uint f = 0;
			switch (round) {
			case 0:
				f = funF(b, c, d);
				break;
			case 1:
				f = funH(b, c, d);
				break;
			case 2:
				f = funG(b, c, d);
				break;
			case 3:
				f = funH(b, c, d);
				break;
			}

			uint t = rotate(a, 5) + f + e + w[j] + y[round];
			e = d;
			d = c;
			c = rotate(b, 30);
			b = a;
			a = t;

			// std::cout << j << ' ';
			// printWorkingVariables(a, b, c, d, e);
		}

		h1 += a;
		h2 += b;
		h3 += c;
		h4 += d;
		h5 += e;
	}

	std::vector<uc> h;
	h = appendBytes(h, h1);
	h = appendBytes(h, h2);
	h = appendBytes(h, h3);
	h = appendBytes(h, h4);
	h = appendBytes(h, h5);
	return h;
}

std::vector<uc> SHA1::hash(int x) {
	return hash(std::to_string(x));
}

std::vector<uc> SHA1::truncHashBin(const std::string& str, int bits) {
	std::vector<uc> hsh = hash(str);
	std::vector<uc> res;

	for (int i = 0; i < bits; ++i) {
		res.push_back(hsh[i]);
	}

	return res;
}

uint SHA1::truncHash(const std::string& str, int bits) {
	std::vector<uc> hsh = hash(str);
	uint res = 0;
	
	for (int i = 0; i < fmin(32, bits); ++i) {
		res = res * 2 + hsh[i];
	}

	return res;
}

uint SHA1::truncHash(int x, int bits) {
	return truncHash(std::to_string(x), bits);
}
