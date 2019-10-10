#pragma once
#include "Figura.h"
#include "Color.h"
#include <vector>
#include <iostream>

class FiguriContainer
{
private:
	std::vector<Figura*>figs;
public:
	void Add (Figura *f);
	void ShowAll ();
	void ShowByColor (Color *c);
};