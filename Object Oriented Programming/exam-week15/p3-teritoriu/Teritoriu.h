#pragma once
#include <string>
#include <iostream>

class Teritoriu
{
protected:
	std::string nume;
	int suprafata;
	void PrintSpaces (int cnt);
public:
	virtual void Afiseaza (int spaces = 0) = 0;
	virtual int CalculeazaSuprafata () = 0;
};