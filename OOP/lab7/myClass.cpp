#include "myClass.h"

inline myClass::myClass (int v1, int v2)
{
	value1 = v1;
	value2 = v2;
}

inline myClass::myClass (std::pair<int, int> v)
{
	value1 = v.first;
	value2 = v.second;
}

void myClass::set (int v1, int v2)
{
	value1 = v1;
	value2 = v2;
}

bool myClass::operator >(const myClass &obj)
{
	return (value1 + value2) > (obj.value1 + obj.value2);
}

std::ostream & operator<<(std::ostream & out, const myClass & obj)
{
	out << obj.value1 << ' ' << obj.value2;
	return out;
}
