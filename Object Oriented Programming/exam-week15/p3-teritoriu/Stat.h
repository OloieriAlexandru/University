#pragma once
#include "Teritoriu.h"
#include "Munti.h"
#include <vector>
#include <iostream>

class Stat : public Teritoriu
{
private:
	std::vector<Munti>munti;
public:
	Stat (std::string nume, int sup);
	void Add (Munti m);

	bool VerificaSuprafataMunti ();

	// Inherited via Teritoriu
	virtual void Afiseaza (int spaces = 0) override;
	virtual int CalculeazaSuprafata () override;
};