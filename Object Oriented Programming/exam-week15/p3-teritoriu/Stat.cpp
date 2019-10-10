#include "Stat.h"

Stat::Stat (std::string nume, int sup)
{
	this->nume = nume;
	this->suprafata = sup;
}

void Stat::Add (Munti m)
{
	munti.push_back (m);
}

bool Stat::VerificaSuprafataMunti ()
{
	int act = 0;
	for (auto x : munti)
	{
		act += x.CalculeazaSuprafata ();
	}
	int need = (30 * suprafata) / 100;
	return act >= need;
}

void Stat::Afiseaza (int spaces)
{
	PrintSpaces (spaces);
	std::cout << "Stat: " << nume << "(" << suprafata << " km2)\n";
	for (auto x : munti)
	{
		x.Afiseaza (spaces+1);
	}
}

int Stat::CalculeazaSuprafata ()
{
	return suprafata;
}
