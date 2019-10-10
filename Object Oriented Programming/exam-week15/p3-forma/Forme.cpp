#include "Forme.h"

void Forme::Add (Forma * f)
{
	forms.push_back (f);
}

void Forme::Paint ()
{
	for (int i = 0; i < (int)forms.size (); ++i)
		forms[i]->Paint ();
}
