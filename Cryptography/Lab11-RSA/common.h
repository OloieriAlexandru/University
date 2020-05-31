#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

#include <NTL/ZZ.h>

int readInt(const std::string& message);

void readBigNumber(NTL::ZZ& num);

void readBigNumber(NTL::ZZ& num, const std::string& message);

void stringToBigNumber(const std::string& str, NTL::ZZ& num);

void bigNumberToString(const NTL::ZZ& num, std::string& str);

std::string readStringInput(const std::string& message);

std::string getRandomString(int len);

void printRSAVariables(const NTL::ZZ& n, const NTL::ZZ& p, const NTL::ZZ& q, const NTL::ZZ& e, const NTL::ZZ& d);
