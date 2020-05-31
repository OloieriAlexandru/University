#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "common.h"

#define uc unsigned char

class DES {
private:
	static const int ip[64]; // initial permutation, IP
	static const int fp[64]; // final permutation, IP^(-1)
	
	static const int et[48]; // the expansion function, E
	static const int sb[8][4][16]; // the eight S-boxes
	static const int pt[32]; // the permutation used to permute the bitstring C

	static const int st[16]; // the shifting table, specifies how many times the two parts of the key should be shifted during each round
	static const int pc1[56]; // the first permutation used in the computation of the round keys
	static const int pc2[48]; // the second permutation used in the computation of the round keys

	std::vector<uc> permute(const std::vector<uc>& arr, const int permutation[]);
	std::vector<uc> expand(const std::vector<uc>& arr, const int fe[], int len);
	std::vector<uc> shift(const std::vector<uc>& arr, int shiftValue);
	std::vector<uc> combine(const std::vector<uc>& a, const std::vector<uc>& b);
	std::vector<uc> xorr (const std::vector<uc>& a, const std::vector<uc>& b);

	std::vector<uc> splitByteArr(const std::vector<uc>& arr, int half);

	std::vector<uc> byteArrRemoveParityBits(const std::vector<uc>& arr);

	std::vector<uc> encryptChunk(const std::string& str, const std::string& key);
	std::vector<uc> encryptChunk(const std::vector<uc>& arr, const std::string& key);
	std::vector<uc> encryptChunk(const std::vector<uc>& arr, const std::vector<uc>& key);

	std::vector<uc> decryptChunk(const std::vector<uc>& arr, const std::vector<uc>& key);
	std::vector<uc> decryptChunk(const std::vector<uc>& arr, const std::string& key);

	std::vector<uc> f(const std::vector<uc>& a, const std::vector<uc>& j);
	std::vector<std::vector<uc>> splitIn6BitArrays(const std::vector<uc>& arr);
	std::vector<uc> intToBinary(int e);
	int getSBoxLine(const std::vector<uc>& arr);
	int getSBoxColumn(const std::vector<uc>& arr);

	void printRoundKeys(const std::vector<std::vector<uc>>& keys);
	std::vector<std::vector<uc>> computeRoundKeys(const std::string& key);
	std::vector<std::vector<uc>> computeRoundKeys(std::vector<uc> key);

public:
	std::vector<uc> encrypt(const std::string& str, const std::string& key);
	std::vector<uc> encrypt(const std::vector<uc>& arr, const std::string& key);
	std::vector<uc> encrypt(const std::vector<uc>& arr, const std::vector<uc>& key);

	std::vector<uc> decrypt(const std::vector<uc>& arr, const std::string& key);
	std::vector<uc> decrypt(const std::vector<uc>& arr, const std::vector<uc>& key);

};
