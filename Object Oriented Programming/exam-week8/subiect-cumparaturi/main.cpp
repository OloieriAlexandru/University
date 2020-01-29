#include <iostream>
#include "ListaCumparaturi.h"
#include "Cumparaturi.h"

int main ()
{
	ListaCumparaturi L;
	(L += Cumparaturi ("paine", 5)) += Cumparaturi ("peste", 2);
	L.Print ();
	L -= "peste";
	L.Print ();
	ListaCumparaturi L2;
	(L2 += Cumparaturi ("lapte", 4)) += Cumparaturi ("paine", -2);
	(L | L2).Print ();
	return 0;
}

/*
Timp implementare: 28 minute
*/