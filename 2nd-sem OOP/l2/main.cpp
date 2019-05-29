#include <iostream>
#include <cstring>
#include "MyString.h"

MyString x;

int main()
{
	x.Set ("Dupa asta ar trebui sa fie inserat ceva: a fost?");
	std::cout << x;
	x.Insert (40, "test test test");
	std::cout << x;
	x.Add ("Am adaugat ceva text la final\n");
	std::cout << x;
	int poz =  x.FindLast ("text");
	int ln = strlen ("text");
	MyString *nou = x.SubString (poz, ln);
	std::cout << *nou << '\n';
	return 0;
}