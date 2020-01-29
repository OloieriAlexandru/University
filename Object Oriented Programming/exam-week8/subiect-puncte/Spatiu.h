#pragma once
#include "Puncte2d.h"
#include <vector>

class Spatiu
{
private:
	std::vector<Puncte2d*>pct;
public:
	Spatiu ();

	void AddPuncte (Puncte2d& x);

	std::vector<Puncte2d*>::iterator begin ();
	std::vector<Puncte2d*>::iterator end ();
};