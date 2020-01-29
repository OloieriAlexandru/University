#pragma once
#include <cstring>
#include <string>

class Propozitie;

class propIterator
{
public:
	int currWord;
	Propozitie *p;
	propIterator& operator++();
	bool operator!=(const propIterator &pI);
	std::string operator*();
};

class Propozitie
{
private:
	char *txt;
	bool matchAtPosI (int n, int m, int i, bool ignoreCase, const char *word);
	bool isLetter (char c1);
	bool isUpper (char c1);
	bool sameChr (char c1, char c2, bool ignoreCase);
public:
	Propozitie ();
	Propozitie (const char *txt);
	~Propozitie ();

	void Create (const char *txt);

	int GetNumberOfWords ();
	bool Contains (const char *word, bool ignoreCase);

	operator char*();
	std::string operator[](int index);

	propIterator begin ();
	propIterator end ();
};