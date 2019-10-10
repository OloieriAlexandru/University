#pragma once
#include "SpatiuInchis.h"
#include<string>
#include<vector>
#include<iostream>

class Pestera : public SpatiuInchis
{
private:
	std::string miros;
	std::string nume;
	bool lumina;
	std::vector<SpatiuInchis*>spaces;
public:
	Pestera (std::string m, std::string n, bool l);

	// Inherited via SpatiuInchis
	virtual void AddSpatiu (SpatiuInchis * si) override;
	virtual void PrintInfo () override;
};