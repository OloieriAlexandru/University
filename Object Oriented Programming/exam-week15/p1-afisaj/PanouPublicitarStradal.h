#pragma once
#include "Afisaj.h"
#include <string>
#include <iostream>
#include <vector>

class PanouPublicitarStradal : public Afisaj
{
private:
	std::string adresa;
	int Lungime;
	int Inaltime;
public:
	PanouPublicitarStradal (std::string adr, int L, int I);

	// Inherited via Afisaj
	virtual void Display () override;

	std::vector<Afisaj*>::iterator begin ();
	std::vector<Afisaj*>::iterator end ();
};