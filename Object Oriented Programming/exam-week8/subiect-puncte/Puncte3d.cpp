#include "Puncte3d.h"

Puncte3d::Puncte3d (std::initializer_list<std::tuple<int, int, int>> l)
{
	nrPuncte = l.size ();
	x = new int[nrPuncte];
	y = new int[nrPuncte];
	z = new int[nrPuncte];
	int i = 0;
	for (auto point : l)
	{
		x[i] = std::get<0>(point);
		y[i] = std::get<1>(point);
		z[i] = std::get<2>(point);
		++i;
	}
}

Puncte3d::~Puncte3d ()
{
	delete[] z;
}

void Puncte3d::SumaAxe ()
{
	int sx = 0, sy = 0, sz = 0;
	for (int i = 0; i < nrPuncte; ++i)
	{
		sx += x[i];
		sy += y[i];
		sz += z[i];
	}
	std::cout << sx << ' ' << sy << ' ' << sz << '\n';
}
