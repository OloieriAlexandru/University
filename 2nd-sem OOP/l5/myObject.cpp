#include "myObject.h"

myObject& myObject::operator^=(const myObject & a)
{
	x ^= a.x;
	y ^= a.y;
	return *this;
}

bool operator>(const myObject & a, const myObject & b)
{
	return a.x * a.x + a.y * a.y > b.x * b.x + b.y * b.y;
}

std::ostream & operator<<(std::ostream & out, const myObject & b)
{
	out << '(' << b.x << ',' << b.y << ')';
	return out;
}

void myObject::set (int x, int y)
{
	this->x = x;
	this->y = y;
}
