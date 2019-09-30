#include "myClass.h"

std::ostream & operator<<(std::ostream & out, const myClass & mC)
{
	out << "{" << mC.x << ", " << mC.y << ", " << mC.w << "}";
	return out;
}

int myClass::getX ()
{
	return x;
}

int myClass::getY ()
{
	return y;
}

int myClass::getW ()
{
	return w;
}
