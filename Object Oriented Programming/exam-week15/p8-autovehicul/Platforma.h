#pragma once
#include "Autovehicul.h"
#include "Autoturism.h"
#include <iostream>

class Platforma : public Autovehicul
{
private:
	Autoturism *a;
public:
	Platforma (std::string m, std::string c);

	void UrcaPePlatforma (Autoturism *a);

	// Inherited via Autovehicul
	virtual void afiseaza () override;
};