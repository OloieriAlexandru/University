#include "Miscellaneous.h"

void Miscellaneous::setCount (int count)
{
	this->count = count;
}

std::string Miscellaneous::getInfo ()
{
	std::string i = std::to_string (count);
	return i + " items";
}
