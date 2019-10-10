#include "Autoturism.h"
#include "Platforma.h"
#include "Transport.h"
#include "Parcare.h"

int main ()
{
	Parcare p ("La nea' Dorel");
	Autoturism a1 ("Dacia Logan", "alb");
	Autoturism a2 ("Renault Megan", "rosu");
	Autoturism a3 ("Ford Focus", "albastru");
	Autoturism a4 ("Volkswagen Passat", "gri");
	Autoturism a5 ("BMW 335i", "alb");
	Autoturism a6 ("Ford Fiesta", "roz");

	p.Adauga (&a1);
	p.Adauga (&a5);
	p.Adauga (&a3);

	Platforma p1 ("Mercedes Sprinter", "alb");
	p1.UrcaPePlatforma (&a2);
	Transport t1 ("Volva FH16", "gri");
	t1.Adauga (&a4);
	t1.Adauga (&a6);

	p.Adauga (&p1);
	p.Adauga (&t1);

	p.AfiseazaColectie ();
	p.AfiseazaDupaCuloare ("alb");

	return 0;
}