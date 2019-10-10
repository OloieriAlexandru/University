#pragma once
#include "BooleanExpr.h"
#include <iostream>

class BoolValue : public BooleanExpr
{
private:
	bool val;
public:
	BoolValue (bool val);

	// Inherited via BooleanExpr
	virtual bool computeResult () override;
	virtual void printExpression (int lvl = 0) override;
	virtual void addExpr (BooleanExpr *exp) override;
};