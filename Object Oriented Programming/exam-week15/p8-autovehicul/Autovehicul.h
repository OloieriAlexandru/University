#pragma once
#include <string>

class Autovehicul
{
protected:
	std::string model;
	std::string culoare;
public:
	virtual void afiseaza () = 0;
	std::string getColor ();
};