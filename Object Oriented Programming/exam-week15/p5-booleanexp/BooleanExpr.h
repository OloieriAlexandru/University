#pragma once

class BooleanExpr
{
public:
	virtual bool computeResult () = 0;
	virtual void printExpression (int lvl = 0) = 0;
	virtual void addExpr (BooleanExpr *exp) = 0;
};