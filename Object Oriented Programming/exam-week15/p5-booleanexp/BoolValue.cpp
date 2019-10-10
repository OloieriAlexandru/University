#include "BoolValue.h"

BoolValue::BoolValue (bool val)
{
	this->val = val;
}

bool BoolValue::computeResult ()
{
	return val;
}

void BoolValue::printExpression (int lvl)
{
	std::cout << (val == true ? "true" : "false");
	if (!lvl)
		std::cout << "\n";
}

void BoolValue::addExpr (BooleanExpr *exp)
{
}
