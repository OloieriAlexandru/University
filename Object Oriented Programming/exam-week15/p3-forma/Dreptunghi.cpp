#include "Dreptunghi.h"

void Dreptunghi::set (int x, int y, int lungime, int latime)
{
	this->x = x;
	this->y = y;
	this->latime = latime;
	this->lungime = lungime;
}

void Dreptunghi::Paint ()
{
	std::cout<<"dreptunghi("<<x<<','<<y<<','<<latime<<','<<lungime<<")\n";
}
