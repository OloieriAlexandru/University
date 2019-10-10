#include "Oval.h"

void Oval::set (int x, int y, int raza1, int raza2)
{
	this->x = x;
	this->y = y;
	this->raza1 = raza1;
	this->raza2 = raza2;
}

void Oval::Paint ()
{
	std::cout << "oval(" << x << ',' << y << ',' << raza1 << ',' << raza2 << ")\n";
}
