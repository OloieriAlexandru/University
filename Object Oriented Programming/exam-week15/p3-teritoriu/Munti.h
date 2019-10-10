#pragma once
#include "Teritoriu.h"
#include <iostream>
#include <string>

class Munti : public Teritoriu
{
private:
	int altitudine;
public:
	Munti (std::string nume, int sup, int alt);
	// Inherited via Teritoriu
	virtual void Afiseaza (int spaces = 0) override;
	virtual int CalculeazaSuprafata () override;
};