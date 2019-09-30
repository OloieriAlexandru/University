#pragma once
#include <string>

class Fruct
{
protected:
	int dimensiune;
	std::string culoare;
public:
	int getDim ();
	std::string getCul ();
};