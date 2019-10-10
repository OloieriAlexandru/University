#pragma once
#include "Autovehicul.h"
#include <iostream>
#include <string>

class Autoturism : public Autovehicul
{
public:
	Autoturism (std::string m, std::string c);

	// Inherited via Autovehicul
	virtual void afiseaza () override;
};