#include "ListaParagrafe.h"

void ListaParagrafe::Add (std::string str)
{
	v.push_back (str);
}

void ListaParagrafe::Add (const char * str)
{
	std::string s (str);
	Add (s);
}
