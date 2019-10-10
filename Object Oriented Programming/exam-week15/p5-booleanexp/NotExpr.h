#pragma once
#include "BooleanExpr.h"
#include <iostream>

class NotExpr : public BooleanExpr
{
	BooleanExpr *expr;
public:
	NotExpr ();
	NotExpr (BooleanExpr *expr);

	// Inherited via BooleanExpr
	virtual bool computeResult () override;
	virtual void printExpression (int lvl = 0) override;
	virtual void addExpr (BooleanExpr * exp) override;
};