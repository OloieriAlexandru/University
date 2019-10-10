#pragma once
#include "Teritoriu.h"
#include "Stat.h"
#include <vector>
#include <iostream>

class Continent : public Teritoriu
{
private:
	std::vector<Stat>state;
public:
	Continent (std::string nume);

	void Add (Stat s);
	void AfiseazaStateCuMunti ();

	// Inherited via Teritoriu
	virtual void Afiseaza (int spaces = 0) override;
	virtual int CalculeazaSuprafata () override;

};