#include <iostream>
#include "SpatiuInchis.h"
#include "Casa.h"
#include "Pestera.h"
#include "Camera.h"

int main ()
{
	SpatiuInchis *c1 = new Camera ("paie", "culoar frontal", true);
	SpatiuInchis *c2 = new Camera ("urs", "camera ursului", false);
	SpatiuInchis *c3 = new Camera ("rugina", "camera ursului", false);
	SpatiuInchis *c4 = new Pestera ("neutru", "Pestera muierilor", false);

	c4->AddSpatiu (c1);
	c4->AddSpatiu (c2);
	c4->AddSpatiu (c3);

	SpatiuInchis *my_home = new Casa ("var", "Casa lui Manole", true);
	my_home->AddSpatiu (new Camera ("mucegai", "Baie", false));
	my_home->AddSpatiu (new Camera ("parfum", "Sufragerie", true));

	c4->PrintInfo ();
	my_home->PrintInfo ();

	return 0;
}