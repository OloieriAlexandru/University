#include "Platforma.h"

Platforma::Platforma (std::string m, std::string c)
{
	model = m;
	culoare = c;
}

void Platforma::UrcaPePlatforma (Autoturism * a)
{
	this->a = a;
}

void Platforma::afiseaza ()
{
	std::cout << "Platforma marca " << model << ", culoarea: " << culoare<<'\n';
	if (a)
	{
		std::cout << "    Transporta ";
		a->afiseaza ();
	}
}
