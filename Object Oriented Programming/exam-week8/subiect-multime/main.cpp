#include <iostream>
#include "Multime.h"
#include "Punct.h"

int main ()
{
	Punct p1 (-2, -2), p2 (2, -3), p3 (1, 1), p4 (8, 8), p5 (1, 1), p6 (1, 12), p7 (2, 4);
	Multime M, M2 = { p5, p6, p7 };
	(((M += p1) += p2) += p3) += p4;
	double d = p1 | p3;
	std::cout << d << std::endl;
	M.distanta_maxima_intre_puncte ();
	M -= p4;
	M.distanta_maxima_intre_puncte ();
	M += M2;
	M.afisare ();
	return 0;
}

/*
Timp implementare: 17 minute
*/