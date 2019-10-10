#pragma once
#include "BooleanExpr.h"
#include <vector>
#include <iostream>
#include <string>

class OrExpr : public BooleanExpr
{
private:
	std::vector<BooleanExpr*>op;
public:
	OrExpr ();

	// Inherited via BooleanExpr
	virtual bool computeResult () override;
	virtual void printExpression (int lvl = 0) override;
	virtual void addExpr (BooleanExpr * exp) override;
};