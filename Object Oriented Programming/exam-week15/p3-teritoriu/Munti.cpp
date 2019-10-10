#include "Munti.h"

Munti::Munti (std::string nume, int sup, int alt)
{
	this->nume = nume;
	this->altitudine = alt;
	this->suprafata = sup;
}

void Munti::Afiseaza (int spaces)
{
	PrintSpaces (spaces);
	std::cout << "Munte: " << nume << "(" << altitudine << " m)\n";
}

int Munti::CalculeazaSuprafata ()
{
	return suprafata;
}
