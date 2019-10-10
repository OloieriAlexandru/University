#pragma once
#include "SpatiuInchis.h"
#include <vector>
#include <iostream>
#include <string>

class Casa : public SpatiuInchis
{
private:
	std::string miros;
	std::string nume;
	bool lumina;
	std::vector<SpatiuInchis*>spaces;
public:
	Casa (std::string m, std::string n, bool l);

	// Inherited via SpatiuInchis
	virtual void AddSpatiu (SpatiuInchis * si) override;
	virtual void PrintInfo () override;
};
