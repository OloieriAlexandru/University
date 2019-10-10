#include "Dacia.h"

void Dacia::setCapacitate (int cap)
{
	capacitate = cap;
}

void Dacia::setCuloare (std::string cul)
{
	culoare = cul;
}

int Dacia::getCapacitate ()
{
	return capacitate;
}

std::string Dacia::getCuloare ()
{
	return culoare;
}

std::string Dacia::getName ()
{
	return "Dacia";
}
