#include <iostream>
#include "Munti.h"
#include "Stat.h"
#include "Continent.h"

int main ()
{
	Continent europa ("Europa");
	Stat elvetia ("Elvatia", 1000);
	Stat italia ("Italia", 6000);
	Stat romania ("Romania", 3000);

	Munti alpi ("Alpi", 400, 3000);
	elvetia.Add (alpi);
	Munti carpati ("Carpati", 200, 2500);
	romania.Add (carpati);

	europa.Add (elvetia);
	europa.Add (italia);
	europa.Add (romania);

	Teritoriu *t = &europa;

	t->Afiseaza ();
	std::cout << "Suprafata Europei: " << t->CalculeazaSuprafata () << " km2" << std::endl;
	std::cout << "Statele din Europa cu peste 30% montan: " << std::endl;
	europa.AfiseazaStateCuMunti ();
	return 0;
}