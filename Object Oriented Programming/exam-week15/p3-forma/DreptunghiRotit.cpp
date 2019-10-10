#include "DreptunghiRotit.h"

void DreptunghiRotit::set (int x, int y, int lungime, int latime, int unghi)
{
	this->x = x;
	this->y = y;
	this->latime = latime;
	this->lungime = lungime;
	this->unghi = unghi;
}

void DreptunghiRotit::Paint ()
{
	std::cout << "dreptunghi(" << x << ',' << y << ',' << lungime << ',' << latime << ") rotit la " << unghi<< " de grade\n";
}
