#include "Cerc.h"

void Cerc::set (int x, int y, int raza)
{
	this->x = x;
	this->y = y;
	this->raza = raza;
}

void Cerc::Paint ()
{
	std::cout << "cerc(" << x << ',' << y << ',' << raza << ")\n";
}
