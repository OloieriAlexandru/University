#pragma once
#include "Procesator.h"
#include <map>
#include <string>
#include <cstring>
#include <iostream>

class ProcesatorNumaraCuvinte : public Procesator
{
private:
	std::map<std::string, int>cnt;
public:
	void Proceseaza (std::string str);
	void Proceseaza (const char *str);
	void Print ();
};