#include "AndExpr.h"

AndExpr::AndExpr ()
{
}

bool AndExpr::computeResult ()
{
	if (op.size () <= 1)
		throw "Error: Illegal number of operands for AND: " + std::to_string (op.size ()) + "operand \n";
	bool ans = op[0]->computeResult ();
	for (int i = 1; i < (int)op.size (); ++i)
		ans = ans && op[i]->computeResult ();
	return ans;
}

void AndExpr::printExpression (int lvl)
{
	if (op.size () <= 1)
		throw "Error: Illegal number of operands for AND: " + std::to_string (op.size ()) + " operand \n";
	std::cout << "&&(";
	for (int i = 0; i < (int)op.size () - 1; ++i)
	{
		op[i]->printExpression (lvl+1);
		std::cout << ", ";
	}
	op[op.size () - 1]->printExpression (lvl+1);
	std::cout << ")";
	if (!lvl)
		std::cout << "\n";
}

void AndExpr::addExpr (BooleanExpr * exp)
{
	op.push_back (exp);
}
