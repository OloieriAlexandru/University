#pragma once
#include <string>

class Figura
{
protected:
	std::string culoare;
public:
	std::string GetCuloare ();
	virtual std::string GetName () = 0;
};