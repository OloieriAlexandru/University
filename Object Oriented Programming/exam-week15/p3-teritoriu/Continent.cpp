#include "Continent.h"

Continent::Continent (std::string nume)
{
	this->nume = nume;
	this->suprafata = 0;
}

void Continent::Add (Stat s)
{
	state.push_back (s);
}

void Continent::AfiseazaStateCuMunti ()
{
	for (auto x : state)
	{
		if (x.VerificaSuprafataMunti ())
		{
			x.Afiseaza (1);
		}
	}
}

void Continent::Afiseaza (int spaces)
{
	PrintSpaces (spaces);
	std::cout << "Continent: " << nume << '\n';
	for (auto x : state)
	{
		x.Afiseaza (spaces+1);
	}
}

int Continent::CalculeazaSuprafata ()
{
	int ans = 0;
	for (auto x : state)
		ans += x.CalculeazaSuprafata ();
	return ans;
}
