#include "ProcesatorNumaraCuvinte.h"
#include <iostream>

void ProcesatorNumaraCuvinte::Proceseaza (std::string str)
{
	int ln = str.size ();
	char *p = new char[ln + 1];
	memset (p, 0, sizeof(char)*(ln+1));
	for (int i = 0; i < ln; ++i)
		p[i] = str[i];
	char *pp = strtok (p, ",!.? ");
	while (pp)
	{
		std::string add (pp);
		++cnt[pp];
		pp = strtok (NULL, ",!.? ");
	}
	delete[] p;
}

void ProcesatorNumaraCuvinte::Proceseaza (const char * str)
{
	std::string s (str);
	Proceseaza (s);
}

void ProcesatorNumaraCuvinte::Print ()
{
	int nr = cnt.size (), i = 0;
	for (auto x : cnt)
	{
		++i;
		std::cout << x.first << ": " << x.second;
		if (i != nr)
			std::cout << ",";
	}	
	std::cout << '\n';
}
