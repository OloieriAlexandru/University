#include "Multime.h"

Multime::Multime ()
{
}

Multime::Multime (std::initializer_list<Punct> ps)
{
	for (auto x : ps)
		points.push_back (x);
}

void Multime::distanta_maxima_intre_puncte ()
{
	double ans = 0, act;
	for (unsigned i = 0; i < points.size () - 1; ++i)
	{
		for (unsigned j = i + 1; j < points.size (); ++j)
		{
			act = points[i] | points[j];
			if (act > ans)
			{
				ans = act;
			}
		}
	}
	std::cout << "Distanta maxima dintre 2 puncte este: " << ans << std::endl;
}

void Multime::afisare ()
{
	for (auto x : points)
	{
		std::cout << x << '\n';
	}
}

Multime & operator+=(Multime & m, const Punct & p)
{
	bool found = false;
	for (unsigned i = 0; i < m.points.size (); ++i)
	{
		if (m.points[i] == p)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		m.points.push_back (p);
	}
	return m;
}

Multime & operator-=(Multime & m, const Punct & p)
{
	int rm = -1;
	for (unsigned i = 0; i < m.points.size (); ++i)
	{
		if (m.points[i] == p)
		{
			rm = i;
			break;
		}
	}
	if (rm != -1)
	{
		m.points.erase (m.points.begin () + rm);
	}
	return m;
}

Multime & operator+=(Multime & m, const Multime & m2)
{
	for (unsigned i = 0; i < m2.points.size (); ++i)
	{
		m += m2.points[i];
	}
	return m;
}
