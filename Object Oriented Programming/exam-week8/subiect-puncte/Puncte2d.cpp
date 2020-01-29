#include "Puncte2d.h"

Puncte2d::Puncte2d ()
{
}

Puncte2d::Puncte2d (std::initializer_list<std::pair<int, int>> l)
{
	nrPuncte = l.size ();
	x = new int[nrPuncte];
	y = new int[nrPuncte];
	int i = 0;
	for (auto pt : l)
	{
		x[i] = pt.first;
		y[i] = pt.second;
		++i;
	}
}

Puncte2d::~Puncte2d ()
{
	delete[] x;
	delete[] y;
}

void Puncte2d::SumaAxe ()
{
	int sx = 0, sy = 0;
	for (int i = 0; i < nrPuncte; ++i)
	{
		sx += x[i];
		sy += y[i];
	}
	std::cout << sx << ' ' << sy << '\n';
}
