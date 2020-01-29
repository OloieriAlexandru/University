#include "Cumparaturi.h"

Cumparaturi::Cumparaturi (std::string t, int c)
{
	type = t;
	cnt = c;
}

std::string Cumparaturi::GetType ()
{
	return type;
}

int Cumparaturi::GetCnt ()
{
	return cnt;
}

void Cumparaturi::AddCnt (int amount)
{
	cnt += amount;
}

std::ostream & operator<<(std::ostream & out, const Cumparaturi & c)
{
	out << c.type << " : " << c.cnt;
	return out;
}