#include "Pestera.h"

Pestera::Pestera (std::string m, std::string n, bool l)
{
	miros = m;
	nume = n;
	lumina = l;
}

void Pestera::AddSpatiu (SpatiuInchis * si)
{
	spaces.push_back (si);
}

void Pestera::PrintInfo ()
{
	std::cout << nume << " are " << spaces.size () << " camere, miros: " << miros << ", " << (lumina == true ? "luminos" : "intunecat")<<'\n';
	for (auto x : spaces)
		x->PrintInfo ();
}
