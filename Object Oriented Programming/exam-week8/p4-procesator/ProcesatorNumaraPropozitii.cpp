#include "ProcesatorNumaraPropozitii.h"

ProcesatorNumaraPropozitii::ProcesatorNumaraPropozitii ()
{
	cnt = 0;
}

void ProcesatorNumaraPropozitii::Proceseaza (std::string str)
{
	int ln = str.size ();
	char *p = new char[ln + 1];
	memset (p, 0, ln+1);
	for (int i = 0; i < ln; ++i)
		p[i] = str[i];
	char sep[] = ".?!";
	char *pp = strtok (p, sep);
	while (pp)
	{
		++cnt;
		pp = strtok (NULL, sep);
	}
	delete[] pp;
}

void ProcesatorNumaraPropozitii::Proceseaza (const char * str)
{
	std::string s (str);
	Proceseaza (s);
}

void ProcesatorNumaraPropozitii::Print ()
{
	std::cout << cnt << " propozitii\n";
}
