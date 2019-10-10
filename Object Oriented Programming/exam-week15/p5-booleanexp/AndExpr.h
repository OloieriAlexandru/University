#pragma once
#include "BooleanExpr.h"
#include <vector>
#include <string>
#include <iostream>

class AndExpr : public BooleanExpr
{
private:
	std::vector<BooleanExpr*>op;
public:
	AndExpr ();

	// Inherited via BooleanExpr
	virtual bool computeResult () override;
	virtual void printExpression (int lvl = 0) override;
	virtual void addExpr (BooleanExpr * exp) override;
};