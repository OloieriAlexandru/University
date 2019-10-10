#include "Color.h"

Color::Color (std::string c)
{
	culoare = c;
}

bool Color::HasColor (std::string col)
{
	return (culoare == col);
}

std::string Color::GetColor ()
{
	return culoare;
}
