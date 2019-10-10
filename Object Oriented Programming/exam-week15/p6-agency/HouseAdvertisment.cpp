#include "HouseAdvertisment.h"

HouseAdvertisment::HouseAdvertisment (int s, int p, int nrf)
{
	SetSurface (s);
	SetPrice (p);
	nrFloors = nrf;
}

void HouseAdvertisment::SetNrFloors (int nr)
{
	nrFloors = nr;
}

void HouseAdvertisment::PrintInfo ()
{
	std::cout << "House with " << nrFloors << " floors, with a surface of " << GetSurface () << " square meters that costs " << GetPrice () << " dollars.\n";
}
