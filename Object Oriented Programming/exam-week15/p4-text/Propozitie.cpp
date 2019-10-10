#include "Propozitie.h"

Propozitie::Propozitie (std::string prop)
{
	for (int i = 0; i < (int)prop.size (); ++i)
	{
		if (prop[i] == ' ') continue;
		int act = i;
		while (i < (int)prop.size () - 1 && prop[i + 1] != ' ')++i;
		Cuvant *add = new Cuvant (prop.substr (act, i - act + 1));
		//std::cout << prop.substr (act, i - act + 1)<<'\n';
		words.push_back (add);
	}
	//std::cout << words.size () << '\n';
}

void Propozitie::AddCuvant (Cuvant &cuv)
{
	words.push_back (&cuv);
}

void Propozitie::Afiseaza ()
{
	for (int i = 0; i < (int)words.size (); ++i)
	{
		words[i]->Afiseaza ();
		std::cout << ' ';
	}
	std::cout << ".";
}
