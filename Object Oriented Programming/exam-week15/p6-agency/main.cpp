#include "Agency.h"
#include "HouseAdvertisment.h"
#include "ApartmentAdveritsment.h"
#include "Advertisment.h"

int main ()
{

	Agency agency ("RealEstate");

	HouseAdvertisment *h1 = new HouseAdvertisment (200, 100000, 3);
	HouseAdvertisment *h2 = new HouseAdvertisment (150, 85000, 3);

	ApartmentAdvertisment *a1 = new ApartmentAdvertisment (60, 50000, 7);
	ApartmentAdvertisment *a2 = new ApartmentAdvertisment (80, 70000, 3);
	ApartmentAdvertisment *a3 = new ApartmentAdvertisment (45, 30000, 5);

	agency.AddAdveritsment (h1);
	agency.AddAdveritsment (h2);
	agency.AddAdveritsment (a1);
	agency.AddAdveritsment (a2);
	agency.AddAdveritsment (a3);

	agency.PrintOffers (100, 1000);
	agency.PrintOffers (50, 99000);

	return 0;
}