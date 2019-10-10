#include "Paragraf.h"

void Paragraf::Add (Text * t)
{
	txts.push_back (t);
}

void Paragraf::Afiseaza ()
{
	for (auto x : txts)
		x->Afiseaza ();
}
