#include "Spatiu.h"

Spatiu::Spatiu ()
{
}

void Spatiu::AddPuncte (Puncte2d & x)
{
	pct.push_back (&x);
}

std::vector<Puncte2d*>::iterator Spatiu::begin ()
{
	return pct.begin ();
}

std::vector<Puncte2d*>::iterator Spatiu::end ()
{
	return pct.end ();
}
