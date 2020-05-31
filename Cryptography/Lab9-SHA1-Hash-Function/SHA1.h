#pragma once

#include <string>
#include <vector>
#include <bitset>

#include "common.h"

#define ull unsigned long long
#define uint unsigned int
#define uc unsigned char

class SHA1 {
private:
	static const uint h1 = 0x67452301;
	static const uint h2 = 0xefcdab89;
	static const uint h3 = 0x98badcfe;
	static const uint h4 = 0x10325476;
	static const uint h5 = 0xc3d2e1f0;
	static const uint y[4];

	std::vector<uc> pad(const std::string& str);
	std::vector<uc> appendBytes(const std::vector<uc>& arr, uint x);

	std::vector<uint> getChunkWords(const std::vector<uc>& bin, int chunk);

	uint rotate(uint x, int pos);
	uint funF(uint a, uint b, uint c);
	uint funG(uint a, uint b, uint c);
	uint funH(uint a, uint b, uint c);

	void printWorkingVariables(uint a, uint b, uint c, uint d, uint e);
public:
	std::vector<uc> getAsBinary(const std::string& str);

	std::vector<uc> hash(const std::string& str);
	std::vector<uc> hash(int x);
	std::vector<uc> truncHashBin(const std::string& str, int bits);
	uint truncHash(const std::string& str, int bits);
	uint truncHash(int x, int bits);
};