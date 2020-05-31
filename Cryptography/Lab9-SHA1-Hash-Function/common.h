#pragma once

#include <map>
#include <string>
#include <vector>
#include <bitset>
#include <iostream>

#define uc unsigned char
#define uint unsigned int

std::string readStringInput(const std::string& message);

static std::map<std::string, char> binaryToHex;

void initBinaryToHexMap();

std::string uintToBinary(uint x);

std::string binaryToHexTransf(const std::vector<uc>& arr);

void printBinaryArr(const std::vector<uc>& arr);