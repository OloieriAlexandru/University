#include "Propozitie.h"

bool Propozitie::matchAtPosI (int n, int m, int i, bool ignoreCase,const char *word)
{
	for (int j = 0; j < m; ++j)
	{
		if (i + j >= n)
		{
			return false;
		}
		if (!sameChr (txt[i + j], word[j], ignoreCase))
		{
			return false;
		}
	}
	return true;
}

bool Propozitie::isLetter (char c1)
{
	return ((c1 >= 'a'&&c1 <= 'z') || (c1 >= 'A'&&c1 <= 'Z'));
}

bool Propozitie::isUpper (char c1)
{
	return c1 >= 'A'&&c1 <= 'Z';
}

bool Propozitie::sameChr (char c1, char c2, bool ignoreCase)
{
	if (ignoreCase)
	{
		if (isLetter (c1) && isLetter (c2))
		{
			if (isUpper (c1))
			{
				c1 = c1 - 'A' + 'a';
			}
			if (isUpper (c2))
			{
				c2 = c2 - 'A' + 'a';
			}
			return c1 == c2;
		}
		else
		{
			return c1 == c2;
		}
	}
	else
	{
		return c1 == c2;
	}
}

Propozitie::Propozitie ()
{
	txt = 0;
}

Propozitie::Propozitie (const char * txt)
{
	int ln = strlen (txt);
	this->txt = new char[ln + 1];
	memset (this->txt, 0, sizeof (txt));
	strcpy (this->txt, txt);
}

Propozitie::~Propozitie ()
{
	delete[] txt;
}

void Propozitie::Create (const char * txt)
{
	if (this->txt)
	{
		delete[] this->txt;
	}
	int ln = strlen (txt);
	this->txt = new char[ln + 1];
	memset (this->txt, 0, sizeof (this->txt));
	strcpy (this->txt, txt);
}

int Propozitie::GetNumberOfWords ()
{
	char *token;
	int ans = 0;
	int ln = strlen (txt);
	char *cpy;
	cpy = new char[ln + 1];
	memset (cpy, 0, sizeof (cpy));
	strcpy (cpy, txt);
	char *p = strtok_s (cpy, " ", &token);
	while (p)
	{
		++ans;
		p = strtok_s (NULL, " ", &token);
	}
	delete[] cpy;
	return ans;
}

bool Propozitie::Contains (const char * word, bool ignoreCase)
{
	int n = strlen (txt);
	int m = strlen (word);
	for (int i = 0; i < n; ++i)
	{
		if (matchAtPosI (n, m, i, ignoreCase, word) && (i==0 || txt[i-1]==' '))
		{
			return true;
		}
	}
	return false;
}

Propozitie::operator char*()
{
	return txt;
}

std::string Propozitie::operator[](int index)
{
	int cnt = -1;
	char *token;
	char *cpy, *p;
	int ln = strlen (txt);
	cpy = new char[ln + 1];
	memset (cpy, 0, sizeof (cpy));
	strcpy (cpy, txt);
	p = strtok_s (cpy, " ", &token);
	while (p)
	{
		++cnt;
		if (cnt == index)
		{
			std::string ret (p);
			for (int i = 0; i < ret.size (); ++i)
			{
				if (isUpper (ret[i]))
				{
					ret[i] = ret[i] - 'A' + 'a';
				}
			}
			delete[] cpy;
			return ret;
		}
		p = strtok_s (NULL, " ", &token);
	}
	delete[] cpy;
	return std::string ("");
}

propIterator Propozitie::begin ()
{
	propIterator it;
	it.currWord = 0;
	it.p = this;
	return it;
}

propIterator Propozitie::end ()
{
	propIterator it;
	it.currWord = GetNumberOfWords ();
	it.p = this;
	return it;
}

propIterator & propIterator::operator++()
{
	++currWord;
	return *this;
}

bool propIterator::operator!=(const propIterator &pI)
{
	return currWord != pI.currWord;
}

std::string propIterator::operator*()
{
	return p->operator[](currWord);
}
