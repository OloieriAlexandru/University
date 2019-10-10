#include "Parcare.h"

Parcare::Parcare (std::string n)
{
	nume = n;
}

void Parcare::AfiseazaColectie ()
{
	std::cout << "Parcarea \"" << nume << "\" contine: \n";
	for (auto x : autos)
		x->afiseaza ();
}

void Parcare::AfiseazaDupaCuloare (std::string c)
{
	std::cout << "Parcarea \"" << nume << "\" contine urmatoarele vehicule de culoarea " << c << ":\n";
	for (auto x : autos)
		if (x->getColor () == c)
			x->afiseaza ();
}

void Parcare::Adauga (Autovehicul * a)
{
	autos.push_back (a);
}
