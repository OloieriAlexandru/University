#include "ApartmentAdveritsment.h"


ApartmentAdvertisment::ApartmentAdvertisment (int s, int p, int l)
{
	SetSurface (s);
	SetPrice (p);
	atLevel = l;
}

void ApartmentAdvertisment::SetLevel (int nr)
{
	atLevel = nr;
}

void ApartmentAdvertisment::PrintInfo ()
{
	std::cout << "Apartment at " << atLevel << "-th level, with a surface of " << GetSurface () << " square meters that costs " << GetPrice () << " dollars.\n";
}
