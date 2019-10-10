#include "Opel.h"

void Opel::setCapacitate (int cap)
{
	capacitate = cap;
}

void Opel::setCuloare (std::string cul)
{
	culoare = cul;
}

void Opel::setAnFabricatie (int an)
{
	anFabricatie = an;
}

int Opel::getAnFabricatie ()
{
	return anFabricatie;
}

int Opel::getCapacitate ()
{
	return capacitate;
}

std::string Opel::getCuloare ()
{
	return culoare;
}

std::string Opel::getName ()
{
	return "Opel";
}
