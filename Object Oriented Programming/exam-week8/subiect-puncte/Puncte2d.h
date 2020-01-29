#pragma once
#include <iostream>
#include <initializer_list>
#include <utility>

class Puncte2d
{
protected:
	int nrPuncte;
	int *x;
	int *y;
public:
	Puncte2d ();
	Puncte2d (std::initializer_list<std::pair<int, int>> l);
	~Puncte2d ();

	virtual void SumaAxe ();
};