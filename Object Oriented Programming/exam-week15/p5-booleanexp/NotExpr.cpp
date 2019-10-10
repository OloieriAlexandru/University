#include "NotExpr.h"

NotExpr::NotExpr ()
{
	expr = 0;
}

NotExpr::NotExpr (BooleanExpr *expr)
{
	this->expr = expr;
}

bool NotExpr::computeResult ()
{
	if (!expr)
		throw "Error: Illegal number of operands\n";
	return !(expr->computeResult ());
}

void NotExpr::printExpression (int lvl)
{
	std::cout << "!(";
	expr->printExpression (lvl+1);
	std::cout << ")";
	if (!lvl)
		std::cout << "\n";
}

void NotExpr::addExpr (BooleanExpr * exp)
{
	expr = exp;
}
