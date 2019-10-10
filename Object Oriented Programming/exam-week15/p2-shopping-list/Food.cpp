#include "Food.h"

void Food::setQuantity (float quantity)
{
	this->quantity = quantity;
}

std::string Food::getInfo ()
{
	std::string q = std::to_string (quantity);
	while (q.back () == '0') q.pop_back ();
	return q + " kg";
}
