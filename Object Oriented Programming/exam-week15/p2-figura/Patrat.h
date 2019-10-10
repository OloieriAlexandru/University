#pragma once
#include "Dreptunghi.h"
#include <string>

class Patrat : public Drepunghi
{
public:
	Patrat (std::string cul);

	virtual std::string GetName () override;
};