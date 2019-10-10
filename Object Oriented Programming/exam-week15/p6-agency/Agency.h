#pragma once
#include "Advertisment.h"
#include <iostream>
#include <vector>

class Agency
{
private:
	const char *name;
	std::vector<Advertisment*>adverts;
public:
	Agency (const char *nm);
	void AddAdveritsment (Advertisment *adv);
	void PrintOffers (int minSurface, int maxPrice);
};