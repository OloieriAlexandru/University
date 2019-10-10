#include "Afisaj.h"
#include "Anunt.h"
#include "PanouPublicitarStradal.h"

int main ()
{
	Afisaj *Panou = new PanouPublicitarStradal ("Anunturi", 10, 10);
	Panou->Add (new Anunt ("Vand apartament: 5 Iunie 2018"));
	Panou->Add (new Anunt ("Cumpar carti: 30 Ianuarie 2017"));
	Panou->Add (new Anunt ("Pierdut caine: 1 Aprilie 2020"));
	Panou->Add (new Anunt ("Ofer meditatii: 6 August 2009"));
	Panou->Display ();
	return 0;
}