#pragma once
#include "Autovehicul.h"
#include <string>
#include <iostream>
#include <vector>

class Transport : public Autovehicul
{
private:
	std::vector<Autovehicul*>autos;
public:
	Transport (std::string m, std::string c);

	void Adauga (Autovehicul *a);

	// Inherited via Autovehicul
	virtual void afiseaza () override;
};