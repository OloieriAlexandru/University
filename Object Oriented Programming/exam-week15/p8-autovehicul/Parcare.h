#pragma once
#include "Autoturism.h"
#include <string>
#include <iostream>
#include <vector>

class Parcare
{
private:
	std::string nume;
	std::vector<Autovehicul*>autos;
public:
	Parcare (std::string n);

	void AfiseazaColectie ();
	void AfiseazaDupaCuloare (std::string c);
	void Adauga (Autovehicul *a);
};