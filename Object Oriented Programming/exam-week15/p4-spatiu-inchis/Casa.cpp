#include "Casa.h"

Casa::Casa (std::string m, std::string n, bool l)
{
	miros = m;
	nume = n;
	lumina = l;
}

void Casa::AddSpatiu (SpatiuInchis * si)
{
	spaces.push_back (si);
}

void Casa::PrintInfo ()
{
	std::cout << nume << " are " << spaces.size () << " camere, miros: " << miros << ", " << (lumina == true ? "luminos" : "intunecat")<<'\n';
	for (auto x : spaces)
		x->PrintInfo();
}
