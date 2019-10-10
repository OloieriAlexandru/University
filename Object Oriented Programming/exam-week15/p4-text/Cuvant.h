#pragma once
#include "Text.h"
#include <string>
#include <iostream>

class Cuvant : public Text
{
private:
	std::string txt;
public:
	Cuvant (std::string txt);
	void SetCuvant (std::string cuvant);

	// Inherited via Text
	virtual void Afiseaza () override;
};