#include <iostream>
#include "Puncte2d.h"
#include "Puncte3d.h"
#include "Spatiu.h"

int main ()
{
	Puncte2d a = { {2,3}, {1, 4}, {5, 6} };
	Puncte3d b = { {1, 5, 3}, {2, 5, 2}, {3, 7, 8}, {1, 0, 1} };

	Spatiu s;
	s.AddPuncte (a);
	s.AddPuncte (b);

	for (auto& x : s)
	{
		x->SumaAxe ();
	}

	return 0;
}