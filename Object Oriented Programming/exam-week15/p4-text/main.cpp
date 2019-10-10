#include "Paragraf.h"
#include "Propozitie.h"
#include "Cuvant.h"

int main ()
{
	Propozitie prop1 ("Ana are mere");
	Propozitie prop2 ("Ionel are cirese");
	Cuvant si ("si");
	Cuvant mere ("mere");
	Cuvant nl ("\n");
	Cuvant tab ("\t");
	Propozitie prop3 ("Vara se coc fructele");

	prop2.AddCuvant (si);
	prop2.AddCuvant (mere);

	Paragraf p;
	p.Add (&prop1);
	p.Add (&nl);
	p.Add (&tab);
	p.Add (&prop2);
	p.Add (&nl);
	p.Add (&prop3);
	p.Add (&nl);
	p.Afiseaza ();

	return 0;
}