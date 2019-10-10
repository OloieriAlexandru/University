#pragma once
#include "Figura.h"
#include <string>

class Cerc : public Figura
{
public:
	Cerc (std::string cul);

	// Inherited via Figura
	virtual std::string GetName () override;
};