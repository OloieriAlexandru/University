#include <iostream>
#include <string>
#include "BooleanExpr.h"
#include "AndExpr.h"
#include "OrExpr.h"
#include "NotExpr.h"
#include "BoolValue.h"

int main ()
{
	try
	{
		BooleanExpr * orr = new OrExpr ();
		orr ->addExpr (new BoolValue (false));
		orr ->addExpr (new BoolValue (true));

		BooleanExpr *andd = new AndExpr ();
		andd->addExpr (new BoolValue (true));
		andd->addExpr (new NotExpr (orr ));
		andd->printExpression ();

		std::cout << std::boolalpha;
		std::cout << andd->computeResult () << std::endl;
		std::cout << orr->computeResult () << std::endl;

		BooleanExpr *andd2 = new AndExpr ();
		andd2->addExpr (new BoolValue (true));
		andd2->printExpression ();
		std::cout << andd2->computeResult () << std::endl;
	}
	catch (const char *errMsg)
	{
		std::cout << errMsg << std::endl;
	}
	catch (std::string errMsg)
	{
		std::cout << errMsg << std::endl;
	}
	return 0;
}