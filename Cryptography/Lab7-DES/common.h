#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <unordered_map>
#include <map>

#define uc unsigned char
#define ull unsigned long long

static std::unordered_map<char, std::string> hexToBinary;

static std::map<std::string, char> binaryToHex;

std::vector<uc> uCharToBinary(uc u);

int binaryToInt(const std::vector<uc>& arr);

std::vector<uc> multiply(int x, int times);

std::vector<uc> multiply(const std::vector<uc>& arr, int times);

void initHexToBinaryMap();

void initBinaryToHexMap();

void trim(std::string& str);

std::vector<uc> readText();

std::vector<uc> readKey(int len);

std::string readOption(std::string message, std::vector<std::string> options);

unsigned char readInt(const std::string& str, int pos);

std::vector<uc> stringToByteArr(const std::string& str);

std::string byteArrToString(const std::vector<uc>& arr);

std::vector<uc> binaryByteArrToByArr(const std::vector<uc>& arr);

std::vector<uc> readASCIIString();

std::vector<uc> readByteArray();

std::vector<uc> readBinaryString();

std::vector<uc> readHEXString();

std::string binaryToHexTransf(const std::vector<uc>& arr);

void printByteArr(const std::vector<uc>& arr);

void printBinaryArr(const std::vector<uc>& arr);

void printInAllForms(const std::vector<uc>& arr);
