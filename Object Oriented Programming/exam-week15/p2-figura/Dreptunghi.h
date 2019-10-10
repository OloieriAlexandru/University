#pragma once
#include "Figura.h"
#include <string>

class Drepunghi : public Figura
{
public:
	Drepunghi (std::string cul);

	// Inherited via Figura
	virtual std::string GetName () override;
};