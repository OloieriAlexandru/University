#include "Agency.h"

Agency::Agency (const char * nm)
{
	name = nm;
}

void Agency::AddAdveritsment (Advertisment * adv)
{
	adverts.push_back (adv);
}

void Agency::PrintOffers (int minSurface, int maxPrice)
{
	int cnt = 0;
	for (auto x : adverts)
	{
		if (x->GetSurface () >= minSurface && x->GetPrice () <= maxPrice)++cnt;
	}
	if (!cnt)
	{
		std::cout << "Agency " << name << " could not find any offer for the criteria: \n";
		std::cout << "    -min surface = " << minSurface << "\n";
		std::cout << "    -max price = " << maxPrice << "\n";
	}
	else
	{
		std::cout << "Agency " << name << " found the following offers: \n";
		for (auto x : adverts)
		{
			if (!(x->GetSurface () >= minSurface && x->GetPrice () <= maxPrice)) continue;
			std::cout << "    ";
			x->PrintInfo ();
		}
	}
}
