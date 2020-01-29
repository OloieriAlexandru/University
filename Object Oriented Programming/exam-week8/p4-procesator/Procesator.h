#pragma once
#include <string>

class Procesator
{
public: 
	virtual void Proceseaza (std::string str) = 0;
	virtual void Proceseaza (const char * str) = 0;
	virtual void Print () = 0;
};