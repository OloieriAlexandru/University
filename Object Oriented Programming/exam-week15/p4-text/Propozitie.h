#pragma once
#include "Text.h"
#include "Cuvant.h"
#include <iostream>
#include <string>
#include <vector>

class Propozitie : public Text
{
private:
	std::vector<Cuvant*>words;
public:
	Propozitie (std::string prop);
	void AddCuvant (Cuvant &cuv);

	// Inherited via Text
	virtual void Afiseaza () override;
};