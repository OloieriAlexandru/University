#include "PanouPublicitarStradal.h"

PanouPublicitarStradal::PanouPublicitarStradal (std::string adr, int L, int I)
{
	adresa = adr;
	Lungime = L;
	Inaltime = I;
}

void PanouPublicitarStradal::Display ()
{
	std::cout << "PANOU_PUBLICITAR: [" << adresa << "] [L:" << Lungime << "] [I:" << Inaltime << "]\n";
	for (auto x : *this)
		x->Display ();
}

std::vector<Afisaj*>::iterator PanouPublicitarStradal::begin ()
{
	return childs.begin ();
}

std::vector<Afisaj*>::iterator PanouPublicitarStradal::end ()
{
	return childs.end ();
}
