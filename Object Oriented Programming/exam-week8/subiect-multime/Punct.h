#pragma once
#include <cmath>
#include <iostream>

class Punct
{
private:
	int x, y;
public:
	Punct ();
	Punct (int x1, int y1);

	friend double operator |(const Punct&a, const Punct &b);
	friend std::ostream& operator <<(std::ostream &out, const Punct &p);

	bool operator ==(const Punct p);
};