#pragma once
#include <string>

class Color
{
private:
	std::string culoare;
public:
	Color (std::string c);

	bool HasColor (std::string col);
	std::string GetColor ();
};