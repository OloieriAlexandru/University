#pragma once
#include <initializer_list>
#include <iostream>
#include <vector>

#include "Punct.h"

class Multime
{
private:
	std::vector<Punct>points;
public:
	Multime ();
	Multime (std::initializer_list<Punct>ps);

	friend Multime& operator +=(Multime &m, const Punct &p);
	friend Multime& operator -=(Multime &m, const Punct &p);
	friend Multime& operator +=(Multime &m, const Multime &m2);

	void distanta_maxima_intre_puncte ();
	void afisare ();
};