#include "..\Simulare 1 2019\Punct.h"
#include "..\Simulare 1 2019\Punct.h"
#include "Punct.h"

Punct::Punct ()
{
}

Punct::Punct (int x1, int y1)
{
	x = x1;
	y = y1;
}

bool Punct::operator==(const Punct p)
{
	return x == p.x && y == p.y;
}

double operator|(const Punct & a, const Punct & b)
{
	return sqrt ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

std::ostream & operator<<(std::ostream & out, const Punct & p)
{
	out << p.x << ' ' << p.y;
	return out;
}
