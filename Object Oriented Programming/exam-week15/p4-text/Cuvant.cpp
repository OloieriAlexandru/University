#include "Cuvant.h"

Cuvant::Cuvant (std::string txt)
{
	this->txt = txt;
}

void Cuvant::SetCuvant (std::string cuvant)
{
	txt = cuvant;
}

void Cuvant::Afiseaza ()
{
	std::cout << txt;
}
